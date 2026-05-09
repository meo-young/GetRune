#include "GRProjectile.h"
#include "NiagaraComponent.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GetRune/GRGameplayTags.h"
#include "GetRune/Player/GRPlayer.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"

AGRProjectile::AGRProjectile()
{
	{
		RC = CreateDefaultSubobject<USceneComponent>(TEXT("RC"));
		SetRootComponent(RC);
	}
	
	// ProjectileMesh를 생성합니다.
	{
		ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
		ProjectileMesh->SetupAttachment(RC);
		ProjectileMesh->SetCollisionProfileName(TEXT("NoCollision"));
	}
	
	// ProjectileEffect를 생성합니다.
	{
		ProjectileEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileEffect"));
		ProjectileEffect->SetupAttachment(ProjectileMesh);
	}
	
	// Collision을 생성합니다.
	{
		Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
		Collision->SetSphereRadius(15.f);
		Collision->SetCollisionProfileName(TEXT("Enemy"));
		Collision->SetupAttachment(ProjectileMesh);
	}

	// ProjectileMovementComponent를 생성합니다.
	{
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
		ProjectileMovement->InitialSpeed = 0.f;
		ProjectileMovement->MaxSpeed = 5000.f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->ProjectileGravityScale = 0.f;
		ProjectileMovement->SetAutoActivate(false);
	}
}

void AGRProjectile::OnActivated()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AGRProjectile::OnSphereOverlap);
	GetWorldTimerManager().SetTimer(LifetimeHandle, this, &AGRProjectile::ReturnToPool, Lifetime, false);
}

void AGRProjectile::OnDeactivated()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collision->OnComponentBeginOverlap.RemoveDynamic(this, &AGRProjectile::OnSphereOverlap);
	GetWorldTimerManager().ClearTimer(LifetimeHandle);
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->Deactivate();
	DamageGE = nullptr;
	SourceASC = nullptr;
	Damage = 0.f;
}

void AGRProjectile::Launch(const FVector& Direction, float InDamage, float InSpeed,
                            TSubclassOf<UGameplayEffect> InDamageGE, UAbilitySystemComponent* InSourceASC)
{
	DamageGE = InDamageGE;
	Damage = InDamage;
	SourceASC = InSourceASC;

	const FVector NormalDir = Direction.GetSafeNormal();
	SetActorRotation(NormalDir.Rotation());
	ProjectileMovement->Velocity = NormalDir * InSpeed;
	ProjectileMovement->Activate();
}

void AGRProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const AGRPlayer* Player = Cast<AGRPlayer>(OtherActor);
	if (!Player || !SourceASC.IsValid() || !DamageGE) return;

	UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
	if (!PlayerASC) return;

	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(DamageGE, 1.f, Context);
	if (Spec.IsValid())
	{
		Spec.Data->SetSetByCallerMagnitude(GRGameplayTags::SetByCaller_Damage, Damage);
		SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), PlayerASC);
	}

	ReturnToPool();
}

void AGRProjectile::ReturnToPool()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (UGRObjectPoolSubsystem* OPS = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UGRObjectPoolSubsystem>())
	{
		OPS->ReleaseActor(this);
	}
}
