#pragma once

#include "CoreMinimal.h"
#include "GetRune/Character/GRCharacter.h"
#include "GetRune/Data/CharacterInfo.h"
#include "GRPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRuneCountChanged, int32, CurrentRuneNum, int32, MaxRuneNum);

class UIndicatorComponent;
class URuneSpawner;
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
	
	
// Delegate
public:
	FOnRuneCountChanged OnRuneCountChanged;


// IAbilitySystem Interface
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override final;
	
	
// Member Function
public:
	bool AddRune(ERuneType RuneType);
	USkillInfo* GetCurrentSkillInfo() const { return CurrentSkillInfo; }
	float GetCurrentDamage() const { return CurrentDamage; }
	AActor* FindNearestEnemy(float Radius) const;

private:
	void Attack();
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
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UIndicatorComponent> IndicatorComponent;
	
	
// Input Variable
private:
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UGRInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TSoftObjectPtr<UInputMappingContext> DefaultMappingContext;


// Attack Variable
private:
	UPROPERTY()
	TMap<ERuneType, FSkillTierData> CachedSkillData;

	UPROPERTY()
	TObjectPtr<USkillInfo> CurrentSkillInfo;
	
	UPROPERTY()
	TObjectPtr<URuneSpawner> RuneSpawner;

	float CurrentDamage = 0.f;
	TMap<ERuneType, int32> RuneCounts;
	TMap<ERuneType, int32> RuneLastAcquired;
	int32 TotalRuneCount = 0;
	int32 RequiredRuneCount = 0;
	
	
};
