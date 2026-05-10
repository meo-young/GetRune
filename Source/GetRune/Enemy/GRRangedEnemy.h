#pragma once

#include "CoreMinimal.h"
#include "GREnemy.h"
#include "GRRangedEnemy.generated.h"

class AGRProjectile;

UCLASS()
class GETRUNE_API AGRRangedEnemy : public AGREnemy
{
	GENERATED_BODY()


// Member Function
public:
	virtual void Attack() override;
	virtual void OnAttackFinished() override;
	
	void FireProjectile();

private:
	void OnAttackCooldownEnd();


// Member Variable
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|몽타주")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "변수|Projectile")
	TSubclassOf<AGRProjectile> ProjectileClass;

private:
	FTimerHandle CooldownHandle;

};
