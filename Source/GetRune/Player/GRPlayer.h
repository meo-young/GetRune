#pragma once

#include "CoreMinimal.h"
#include "GetRune/Character/GRCharacter.h"
#include "GetRune/Data/CharacterInfo.h"
#include "GRPlayer.generated.h"

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
public:
	void AddRune(ERuneType RuneType);
	USkillInfo* GetCurrentSkillInfo() const { return CurrentSkillInfo; }

private:
	void Attack();
	void StartAimPhase(ESkillType SkillType);
	void FireSkill();
	int32 GetCurrentTier() const;
	ERuneType GetDominantRuneType() const;
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_MoveCompleted(const FInputActionValue& InputActionValue);
	
	
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
	
	
// Input Variable
private:
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UGRInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TSoftObjectPtr<UInputMappingContext> DefaultMappingContext;


// Attack Variable
private:
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격")
	float AimDuration = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "변수|공격")
	float InstantDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "변수|공격")
	float SlowMoScale = 0.05f;
	
private:
	UPROPERTY()
	TMap<ERuneType, FSkillTierData> CachedSkillData;

	UPROPERTY()
	TObjectPtr<USkillInfo> CurrentSkillInfo;

	TMap<ERuneType, int32> RuneCounts;
	TMap<ERuneType, int32> RuneLastAcquired;
	int32 TotalRuneCount = 0;
	int32 RequiredRuneCount = 0;

	bool bIsAiming = false;
	FTimerHandle AimTimerHandle;


	
};
