#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GRPlayerController.generated.h"

UCLASS()
class GETRUNE_API AGRPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
};
