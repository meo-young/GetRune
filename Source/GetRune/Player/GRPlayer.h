#pragma once

#include "CoreMinimal.h"
#include "GetRune/Character/GRCharacter.h"
#include "GetRune/Data/CharacterInfo.h"
#include "GRPlayer.generated.h"

class AGRPlayerCameraManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRuneCountChanged, int32, CurrentRuneNum, int32, MaxRuneNum);

class UIndicatorComponent;
class URuneSpawner;
class UNiagaraSystem;
class USphereComponent;
struct FInputActionValue;
class UGRInputConfig;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class AGRProjectilePiercing;
class UGRSkillCameraShake;

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
	void LaunchProjectile();
	AActor* FindNearestEnemy(float Radius) const;

	
protected:
	virtual void OnHealthChanged(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator) override;
	USkillInfo* GetCurrentSkillInfo() const { return CurrentSkillInfo; }
	float GetCurrentDamage() const { return CurrentDamage; }
	UNiagaraSystem* GetCurrentAttackEffect() const { return CurrentAttackEffect; }
	int32 GetLastAttackTier() const { return LastAttackTier; }

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
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격")
	TSubclassOf<AGRProjectilePiercing> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "변수|공격", meta = (DisplayName = "적 탐색 반경"))
	float EnemySearchRadius = 2000.f;

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

	UPROPERTY()
	TObjectPtr<UNiagaraSystem> CurrentAttackEffect;
	int32 LastAttackTier = 1;
	
	UPROPERTY()
	TObjectPtr<AGRPlayerCameraManager> PCM;
	
	
};
