#pragma once

#include "CoreMinimal.h"
#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"
#include "GetRune/Character/GRCharacter.h"
#include "GetRune/Interface/Poolable.h"
#include "GREnemy.generated.h"

class UWidgetComponent;
class UGREnemyStatusWidget;
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

protected:
	virtual void HandleDeath(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator) override;
	virtual void OnHealthChanged(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator) override;

	
private:
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

	void ResetDamageReaction();


// Component
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGRAbilitySystemComponent> ASC;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> StatusWidget;


// Getter
public:
	FORCEINLINE float GetAttackRange() const { return AttackRange; }


// Member Variable
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|GE")
	TSubclassOf<UGameplayEffect> ContactDamageGE;
	
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
