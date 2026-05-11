#pragma once

#include "CoreMinimal.h"
#include "GRProjectile.h"
#include "GRProjectilePiercing.generated.h"

UCLASS()
class GETRUNE_API AGRProjectilePiercing : public AGRProjectile
{
	GENERATED_BODY()

protected:
	virtual void OnActivated() override;
	virtual void HandleOverlap(AActor* OtherActor) override;

private:
	TSet<TWeakObjectPtr<AActor>> HitActors;
};
