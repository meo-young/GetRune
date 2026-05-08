#pragma once

#include "CoreMinimal.h"
#include "GetRune/Character/GRCharacter.h"
#include "GRPlayer.generated.h"

class UGRRuneSpawnComponent;
class USphereComponent;
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
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	
// IAbilitySystem Interface	
public:	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override final;
	
	
// Member Function
private:
	void Input_Move(const FInputActionValue& InputActionValue);
	
	
// Collision Overlap Binding
private:
	UFUNCTION()
	void OnMagnetBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
// Component
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> MagnetCollision;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGRRuneSpawnComponent> RuneSpawnComponent;
	
	
// Input Variable	
private:
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UGRInputConfig> InputConfig;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TSoftObjectPtr<UInputMappingContext> DefaultMappingContext;
	
};
