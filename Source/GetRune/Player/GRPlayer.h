#pragma once

#include "CoreMinimal.h"
#include "GetRune/Character/GRCharacter.h"
#include "GRPlayer.generated.h"

struct FInputActionValue;
class UGRInputConfig;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;

UCLASS()
class GETRUNE_API AGRPlayer : public AGRCharacter
{
	GENERATED_BODY()
	
// LifeSection	
public:
	AGRPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	
// IAbilitySystem Interface	
public:	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override final;
	
	
// Member Function
private:
	void Input_Move(const FInputActionValue& InputActionValue);
	
	
// Component
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	
// Input Variable	
private:
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UGRInputConfig> InputConfig;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TSoftObjectPtr<UInputMappingContext> DefaultMappingContext;
	
};
