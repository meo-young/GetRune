#pragma once

#include "CoreMinimal.h"
#include "GREnemy.h"
#include "GRBossEnemy.generated.h"

UCLASS()
class GETRUNE_API AGRBossEnemy : public AGREnemy
{
	GENERATED_BODY()

protected:
	virtual void HandleDeath(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator) override;
};
