#include "GREnemy.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffect.h"
#include "Components/WidgetComponent.h"
#include "GetRune/GetRune.h"
#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"
#include "GetRune/AbilitySystem/Attributes/GRCombatSet.h"
#include "GetRune/AbilitySystem/Attributes/GRCharacterStatSet.h"
#include "GetRune/AbilitySystem/Attributes/GRHealthSet.h"
#include "GetRune/Component/HealthComponent.h"
#include "GetRune/Data/StageInfo.h"
#include "GetRune/GRGameplayTags.h"
#include "GetRune/GameState/GRGameState.h"
#include "GetRune/Item/GRItemBase.h"
#include "GetRune/Player/GRPlayer.h"
#include "GetRune/Spawner/EnemySpawner.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"
#include "GetRune/Subsystem/SoundSubsystem.h"
#include "GetRune/UI/GREnemyCounterWidget.h"
#include "GetRune/UI/GRHUD.h"
#include "GetRune/UI/GREnemyStatusWidget.h"
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
	
	// StatusWidget을 설정합니다.
	{
		StatusWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusWidget"));
		StatusWidget->SetupAttachment(GetRootComponent());
		StatusWidget->SetWidgetSpace(EWidgetSpace::Screen);
	}
}

void AGREnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGREnemy::OnCapsuleBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AGREnemy::OnCapsuleEndOverlap);
}

void AGREnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
}

void AGREnemy::OnActivated()
{
	ASC->SetLooseGameplayTagCount(GRGameplayTags::Status_Death, 0);
	GetCharacterMovement()->bUseRVOAvoidance = true;
	
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
	HealthComponent->UninitializeWithAbilitySystem();
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

	HealthComponent->InitializeWithAbilitySystem(ASC);
	
	if (UGREnemyStatusWidget* EnemyStatusWidgetInstance = Cast<UGREnemyStatusWidget>(StatusWidget->GetUserWidgetObject()))
	{
		EnemyStatusWidgetInstance->SetHealthRatio(1.f);
	}
}

void AGREnemy::HandleDeath(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator)
{
	StopAnimMontage();
	
	USoundSubsystem::Get(this).PlaySFXByName(FName("OnEnemyDead"));

	if (AAIController* AIC = GetController<AAIController>())
	{
		if (UBrainComponent* Brain = AIC->GetBrainComponent())
		{
			Brain->StopLogic("Death");
		}
	}

	ASC->SetLooseGameplayTagCount(GRGameplayTags::Status_Death, 1);

	GetCharacterMovement()->bUseRVOAvoidance = false;
	GetWorldTimerManager().ClearTimer(ContactCooldownHandle);

	GetWorld()->GetGameState<AGRGameState>()->EnemySpawnManager->DecrementEnemyNum();

	DropItems();

	Super::HandleDeath(HC, OldValue, NewValue, InInstigator);
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

void AGREnemy::OnHealthChanged(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator)
{
	if (NewValue < OldValue)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(FName("Damage.Reaction.Intensity"), 1.f);
		GetWorldTimerManager().SetTimer(DamageReactionHandle, this, &AGREnemy::ResetDamageReaction, 0.2f, false);
		GetMesh()->SetRelativeScale3D(FVector(0.7f, 1.0f, 1.1f));

		if (UGREnemyStatusWidget* EnemyStatusWidgetInstance = Cast<UGREnemyStatusWidget>(StatusWidget->GetUserWidgetObject()))
		{
			EnemyStatusWidgetInstance->SetHealthRatio(HC->GetHealthNormalized());
		}
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
