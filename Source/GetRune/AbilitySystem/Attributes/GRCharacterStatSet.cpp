#include "GRCharacterStatSet.h"

UGRCharacterStatSet::UGRCharacterStatSet()
	: AttackPower(10.0f)
{
}

void UGRCharacterStatSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UGRCharacterStatSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UGRCharacterStatSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetAttackPowerAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}
