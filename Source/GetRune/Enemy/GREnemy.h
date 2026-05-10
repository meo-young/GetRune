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
	void FinishDeath();
	virtual void Attack() {}
	virtual void OnAttackFinished() {}

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

	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void ResetDamageReaction();


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

	UPROPERTY(EditDefaultsOnly, Category = "변수|몽타주")
	TObjectPtr<UAnimMontage> DeathMontage;
	
protected:
	UPROPERTY()
	const UEnemyInfo* CurrentEnemyInfo = nullptr;
	
	bool bCanAttack = true;
	float AttackRange = 0.f;

	FTimerHandle ContactCooldownHandle;
	FTimerHandle DamageReactionHandle;
	TWeakObjectPtr<AGRPlayer> OverlappingPlayer;
	
public:
	FORCEINLINE bool CanAttack() const { return bCanAttack; }

};
