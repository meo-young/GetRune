#pragma once

#include "CoreMinimal.h"
#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"
#include "GetRune/Character/GRCharacter.h"
#include "GetRune/Interface/Poolable.h"
#include "GREnemy.generated.h"

class AGRPlayer;
class UEnemyInfo;
class UGameplayEffect;
struct FGameplayEffectSpec;

UCLASS()
class GETRUNE_API AGREnemy : public AGRCharacter, public IPoolable
{
	GENERATED_BODY()

// Lifecycle
public:
	AGREnemy();
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;


// IAbilitySystemInterface
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }


// IPoolable
public:
	virtual void OnActivated() override;
	virtual void OnDeactivated() override;


// Member Function
public:
	void InitializeFromEnemyInfo(const UEnemyInfo* EnemyInfo);

private:
	void HandleDeath(AActor* InInstigator, AActor* Causer, const FGameplayEffectSpec* Spec, float Magnitude, float OldValue, float NewValue);
	void DropItems() const;

	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ApplyContactDamage(UAbilitySystemComponent* PlayerASC);
	void OnContactDamageTimer();


// Component
protected:
	UPROPERTY()
	TObjectPtr<UGRAbilitySystemComponent> ASC;


// Getter
public:
	FORCEINLINE float GetAttackRange() const { return AttackRange; }


// Member Variable
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|GE")
	TSubclassOf<UGameplayEffect> ContactDamageGE;
	
private:
	UPROPERTY()
	const UEnemyInfo* CurrentEnemyInfo = nullptr;
	
	FDelegateHandle DeathDelegateHandle;
	float AttackRange = 0.f;
	
	FTimerHandle ContactCooldownHandle;
	TWeakObjectPtr<AGRPlayer> OverlappingPlayer;

};
