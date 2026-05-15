#pragma once

#include "CoreMinimal.h"
#include "GREnemy.h"
#include "GRBossEnemy.generated.h"

UCLASS()
class GETRUNE_API AGRBossEnemy : public AGREnemy
{
	GENERATED_BODY()


// Member Function
public:
	virtual void Attack() override;
	virtual void OnAttackFinished() override;

protected:
	virtual void HandleDeath(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator) override;

private:
	void OnAttackCooldownEnd();


// Member Variable
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|몽타주")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

private:
	FTimerHandle CooldownHandle;

};
