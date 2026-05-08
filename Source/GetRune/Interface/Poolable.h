#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Poolable.generated.h"

UINTERFACE()
class UPoolable : public UInterface
{
	GENERATED_BODY()
};

class GETRUNE_API IPoolable
{
	GENERATED_BODY()

public:
	virtual void OnActivated() = 0;
	virtual void OnDeactivated() = 0;
};
