#include "GRCharacter.h"
#include "GRCharacterMovementComponent.h"
#include "GetRune/GRGameplayTags.h"
#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"
#include "GetRune/AbilitySystem/Attributes/GRHealthSet.h"

AGRCharacter::AGRCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGRCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
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
	
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		UGRHealthSet::GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);

	if (const UGRHealthSet* HealthSet = GetAbilitySystemComponent()->GetSet<UGRHealthSet>())
	{
		HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleDeath);
	}
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

void AGRCharacter::HandleDeath(AActor* InInstigator, AActor* Causer, const FGameplayEffectSpec* Spec, float Magnitude, float OldValue, float NewValue)
{
	if (OnCharacterDeath.IsBound())
	{
		OnCharacterDeath.Broadcast();
	}
}

void AGRCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
}
