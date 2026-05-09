#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GRAttributeSet.h"
#include "GRCombatSet.generated.h"

UCLASS()
class GETRUNE_API UGRCombatSet : public UGRAttributeSet
{
	GENERATED_BODY()
	
// Lifecycle	
public:
	UGRCombatSet();

	
// Member Variable	
private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;
	
	
// Getter, Setter	
public:	
	ATTRIBUTE_ACCESSORS(UGRCombatSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UGRCombatSet, BaseHeal);
	
};
