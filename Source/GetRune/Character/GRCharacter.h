#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GRCharacter.generated.h"

class UHealthComponent;
class UMotionWarpingComponent;
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
	UFUNCTION()
	virtual void HandleDeath(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator);

	UFUNCTION()
	virtual void OnHealthChanged(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator);

	void SetMovementModeTag(EMovementMode CurrentMovementMode, uint8 CurrentCustomMode, bool bTagEnabled);


// Component
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

};
