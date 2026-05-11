#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GRCharacter.generated.h"

struct FOnAttributeChangeData;
struct FGameplayEffectSpec;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeath);

UCLASS()
class GETRUNE_API AGRCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
// Lifecycle Section
public:
	AGRCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	virtual void BeginPlay() override;
	
	
// IAbilitySystemInterface	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	
// Delegate	
public:
	UPROPERTY(BlueprintAssignable)
	FOnCharacterDeath OnCharacterDeath;
	
	
// Member Function	
protected:
	virtual void HandleDeath(AActor* InInstigator, AActor* Causer, const FGameplayEffectSpec* Spec, float Magnitude, float OldValue, float NewValue);
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	void SetMovementModeTag(EMovementMode CurrentMovementMode, uint8 CurrentCustomMode, bool bTagEnabled);

	
};
