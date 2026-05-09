#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GRAttributeSet.h"
#include "GRCharacterStatSet.generated.h"

UCLASS()
class GETRUNE_API UGRCharacterStatSet : public UGRAttributeSet
{
	GENERATED_BODY()

// Lifecycle
public:
	UGRCharacterStatSet();
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;


// Member Function
private:
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;


// Member Variable
private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackPower;


// Getter, Setter	
public:
	ATTRIBUTE_ACCESSORS(UGRCharacterStatSet, AttackPower);

};
