#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GRAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class UGRAbilitySystemComponent;
struct FGameplayEffectSpec;
DECLARE_MULTICAST_DELEGATE_SixParams(FGRAttributeEvent, 
		AActor*						/*EffectInstigator*/, 
		AActor*						/*EffectCauser*/,
		const FGameplayEffectSpec*	/*EffectSpec*/, 
		float						/*EffectMagnitude*/,
		float						/*OldValue*/, 
		float						/*NewValue*/
	);

UCLASS()
class GETRUNE_API UGRAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	
public:
	UGRAttributeSet();
	virtual UWorld* GetWorld() const override;
	UGRAbilitySystemComponent* GetGRAbilitySystemComponent() const;
};
