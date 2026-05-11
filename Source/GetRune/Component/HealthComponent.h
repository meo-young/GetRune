#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UGRHealthSet;
struct FGameplayEffectSpec;
class UGRAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealth_DeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHealth_AttributeChanged, UHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GETRUNE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();
	virtual void OnUnregister() override;
	static UHealthComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UHealthComponent>() : nullptr); }
	
public:
	void InitializeWithAbilitySystem(UGRAbilitySystemComponent* InASC);
	void UninitializeWithAbilitySystem();
	float GetHealth() const;
	float GetMaxHealth() const;
	float GetHealthNormalized() const;
	
public:
	FHealth_AttributeChanged OnHealthChanged;
	FHealth_AttributeChanged OnMaxHealthChanged;
	FHealth_AttributeChanged OnDeath;
	
protected:
	virtual void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	virtual void HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	
protected:
	UPROPERTY()
	TObjectPtr<UGRAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<const UGRHealthSet> HealthSet;
	
};
