#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GRAttributeSet.h"
#include "GRHealthSet.generated.h"

UCLASS()
class GETRUNE_API UGRHealthSet : public UGRAttributeSet
{
	GENERATED_BODY()
	
// Lifecycle	
public:
	UGRHealthSet();
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	

// Delegate	
public:
	/** 현재 체력이 바뀌었을 때 호출되는 델리게이트입니다. */
	mutable FGRAttributeEvent OnHealthChanged;
	
	/** 최대 체력이 바뀌었을 때 호출되는 델리게이트입니다. */
	mutable FGRAttributeEvent OnMaxHealthChanged;
	
	/** 플레이어 사망 시 호출되는 델리게이트입니다. */
	mutable FGRAttributeEvent OnOutOfHealth;
	
	
// Member Function	
private:	
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

	
// Member Variable	
private:
	UPROPERTY(BlueprintReadOnly, Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	/** 현재 체력 회복 연산을 위한 속성 값입니다. */
	UPROPERTY(BlueprintReadOnly, Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Healing;

	/** 현재 체력 감소 연산을 위한 속성 값입니다. */
	UPROPERTY(BlueprintReadOnly, Meta=(HideFromModifiers, AllowPrivateAccess=true))
	FGameplayAttributeData Damage;
	
	bool bOutOfHealth;

	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;

	
// Getter, Setter	
public:
	ATTRIBUTE_ACCESSORS(UGRHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UGRHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UGRHealthSet, Healing);
	ATTRIBUTE_ACCESSORS(UGRHealthSet, Damage);
	
};
