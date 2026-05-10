#pragma once

#include "CoreMinimal.h"
#include "GRProjectile.h"
#include "GRProjectileSingle.generated.h"

UCLASS()
class GETRUNE_API AGRProjectileSingle : public AGRProjectile
{
	GENERATED_BODY()

protected:
	virtual void HandleOverlap(AActor* OtherActor) override;
};
