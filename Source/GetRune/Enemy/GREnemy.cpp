#include "GREnemy.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffect.h"
#include "GetRune/GetRune.h"
#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"
#include "GetRune/AbilitySystem/Attributes/GRCombatSet.h"
#include "GetRune/AbilitySystem/Attributes/GRCharacterStatSet.h"
#include "GetRune/AbilitySystem/Attributes/GRHealthSet.h"
#include "GetRune/Data/StageInfo.h"
#include "GetRune/GRGameplayTags.h"
#include "GetRune/Item/GRItemBase.h"
#include "GetRune/Player/GRPlayer.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"

AGREnemy::AGREnemy()
{
	// AbilitySystemComponent를 생성합니다.
	ASC = CreateDefaultSubobject<UGRAbilitySystemComponent>(TEXT("ASC"));
	CreateDefaultSubobject<UGRHealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<UGRCombatSet>(TEXT("CombatSet"));
	CreateDefaultSubobject<UGRCharacterStatSet>(TEXT("CharacterStatSet"));
	
	// AutoPossessAI를 설정합니다.
	{
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}
	
	// CharacterMovement 속성 값을 설정합니다.
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseRVOAvoidance = true;
		GetCharacterMovement()->AvoidanceConsiderationRadius = 200.f;
	}
	
	// CollisionProfile을 설정합니다.
	{
		GetMesh()->SetCollisionProfileName("NoCollision");
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	}
	
	// 회전 속성을 설정합니다.
	{
		bUseControllerRotationYaw = false;
	}
}

void AGREnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGREnemy::OnCapsuleBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AGREnemy::OnCapsuleEndOverlap);

	ASC->GetGameplayAttributeValueChangeDelegate(
		UGRHealthSet::GetHealthAttribute()).AddUObject(this, &AGREnemy::OnHealthChanged);

	if (const UGRHealthSet* HealthSet = ASC->GetSet<UGRHealthSet>())
	{
		HealthSet->OnOutOfHealth.AddUObject(this, &AGREnemy::HandleDeath);
	}
}

void AGREnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
}

void AGREnemy::OnActivated()
{
	if (AAIController* AIC = GetController<AAIController>())
	{
		if (UBrainComponent* Brain = AIC->GetBrainComponent())
		{
			Brain->RestartLogic();
		}
	}
}

void AGREnemy::OnDeactivated()
{
	GetWorldTimerManager().ClearTimer(ContactCooldownHandle);
	GetWorldTimerManager().ClearTimer(DamageReactionHandle);
	GetMesh()->SetScalarParameterValueOnMaterials(FName("Damage.Reaction.Intensity"), 0.f);
	CurrentEnemyInfo = nullptr;
}

void AGREnemy::InitializeFromEnemyInfo(const UEnemyInfo* EnemyInfo)
{
	if (!EnemyInfo) return;
	CurrentEnemyInfo = EnemyInfo;

	GetCharacterMovement()->MaxWalkSpeed = EnemyInfo->MoveSpeed;
	AttackRange = EnemyInfo->AttackRange;

	ASC->SetNumericAttributeBase(UGRHealthSet::GetMaxHealthAttribute(), EnemyInfo->Health);
	ASC->SetNumericAttributeBase(UGRHealthSet::GetHealthAttribute(), EnemyInfo->Health);
	ASC->SetNumericAttributeBase(UGRCharacterStatSet::GetAttackPowerAttribute(), EnemyInfo->AttackPower);

}

void AGREnemy::HandleDeath(AActor* InInstigator, AActor* Causer, const FGameplayEffectSpec* Spec, float Magnitude, float OldValue, float NewValue)
{
	if (AAIController* AIC = GetController<AAIController>())
	{
		if (UBrainComponent* Brain = AIC->GetBrainComponent())
		{
			Brain->StopLogic("Death");
		}
	}

	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().ClearTimer(ContactCooldownHandle);

	DropItems();

	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDeath();
	}
}

void AGREnemy::FinishDeath()
{
	UGRObjectPoolSubsystem* OPS = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UGRObjectPoolSubsystem>();
	OPS->ReleaseActor(this);
}

void AGREnemy::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGRPlayer* Player = Cast<AGRPlayer>(OtherActor);
	if (!Player) return;

	OverlappingPlayer = Player;
	ApplyContactDamage(Player->GetAbilitySystemComponent());
	GetWorldTimerManager().SetTimer(ContactCooldownHandle, this, &AGREnemy::OnContactDamageTimer, 0.5f, true);
}

void AGREnemy::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != OverlappingPlayer.Get()) return;

	GetWorldTimerManager().ClearTimer(ContactCooldownHandle);
	OverlappingPlayer = nullptr;
}

void AGREnemy::OnContactDamageTimer()
{
	if (!OverlappingPlayer.IsValid()) return;
	ApplyContactDamage(OverlappingPlayer->GetAbilitySystemComponent());
}

void AGREnemy::ApplyContactDamage(UAbilitySystemComponent* PlayerASC)
{
	if (!PlayerASC || !ContactDamageGE || !CurrentEnemyInfo) return;

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(ContactDamageGE, 1.f, Context);
	if (Spec.IsValid())
	{
		Spec.Data->SetSetByCallerMagnitude(GRGameplayTags::SetByCaller_Damage, CurrentEnemyInfo->ContactDamage);
		ASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), PlayerASC);
	}
}

void AGREnemy::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue < Data.OldValue)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(FName("Damage.Reaction.Intensity"), 1.f);
		GetWorldTimerManager().SetTimer(DamageReactionHandle, this, &AGREnemy::ResetDamageReaction, 0.2f, false);
		GetMesh()->SetRelativeScale3D(FVector(0.7f, 1.0f, 1.1f));
	}
}

void AGREnemy::ResetDamageReaction()
{
	GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	GetMesh()->SetScalarParameterValueOnMaterials(FName("Damage.Reaction.Intensity"), 0.f);
}

void AGREnemy::DropItems() const
{
	if (!CurrentEnemyInfo) return;

	UGRObjectPoolSubsystem* OPS = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UGRObjectPoolSubsystem>();
	const FVector DropLocation = GetActorLocation();

	for (const TPair<TSubclassOf<AGRItemBase>, float>& Drop : CurrentEnemyInfo->DroppedItems)
	{
		if (FMath::RandRange(0.f, 100.f) <= Drop.Value)
		{
			OPS->AcquireActor(Drop.Key, DropLocation, FRotator::ZeroRotator);
		}
	}
}
