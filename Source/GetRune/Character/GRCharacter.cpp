#include "GRCharacter.h"
#include "GRCharacterMovementComponent.h"
#include "GetRune/GRGameplayTags.h"
#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"
#include "GetRune/Component/HealthComponent.h"
#include "MotionWarpingComponent.h"

AGRCharacter::AGRCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGRCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void AGRCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	const UGRCharacterMovementComponent* MoveComp = Cast<UGRCharacterMovementComponent>(GetCharacterMovement());

	SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
	SetMovementModeTag(MoveComp->MovementMode, MoveComp->CustomMovementMode, true);
}

void AGRCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
	HealthComponent->OnDeath.AddDynamic(this, &ThisClass::HandleDeath);
}

UAbilitySystemComponent* AGRCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

void AGRCharacter::SetMovementModeTag(EMovementMode CurrentMovementMode, uint8 CurrentCustomMode, bool bTagEnabled)
{
	if (UGRAbilitySystemComponent* ASC = Cast<UGRAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		const FGameplayTag* MovementModeTag;

		if (CurrentMovementMode == MOVE_Custom)
		{
			MovementModeTag = GRGameplayTags::CustomMovementModeTagMap.Find(CurrentCustomMode);
		}
		else
		{
			MovementModeTag = GRGameplayTags::MovementModeTagMap.Find(CurrentMovementMode);
		}

		if (MovementModeTag && MovementModeTag->IsValid())
		{
			ASC->SetLooseGameplayTagCount(*MovementModeTag, (bTagEnabled ? 1 : 0));
		}
	}
}

void AGRCharacter::HandleDeath(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator)
{
	OnCharacterDeath.Broadcast();
}

void AGRCharacter::OnHealthChanged(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator)
{
}
