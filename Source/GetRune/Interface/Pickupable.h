#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Pickupable.generated.h"

UINTERFACE()
class UPickupable : public UInterface
{
	GENERATED_BODY()
};

class GETRUNE_API IPickupable
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnMagnetOverlapped() = 0;
	
	UFUNCTION()
	virtual void OnPlayerOverlapped() = 0;
	
};
