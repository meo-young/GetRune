#include "GRCharacter.h"
#include "GRCharacterMovementComponent.h"
#include "GetRune/GRGameplayTags.h"
#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"

AGRCharacter::AGRCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGRCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
}

void AGRCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	const UGRCharacterMovementComponent* MoveComp = CastChecked<UGRCharacterMovementComponent>(GetCharacterMovement());
	
	SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
	SetMovementModeTag(MoveComp->MovementMode, MoveComp->CustomMovementMode, true);
}

UAbilitySystemComponent* AGRCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

void AGRCharacter::SetMovementModeTag(EMovementMode CurrentMovementMode, uint8 CurrentCustomMode, bool bTagEnabled)
{
	if (UGRAbilitySystemComponent* ASC = CastChecked<UGRAbilitySystemComponent>(GetAbilitySystemComponent()))
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
