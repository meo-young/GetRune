#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GRAIController.generated.h"

UCLASS()
class GETRUNE_API AGRAIController : public AAIController
{
	GENERATED_BODY()
	
// Lifecycle	
public:
	AGRAIController();
	virtual void OnPossess(APawn* InPawn) override;
	

// Member Variable	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
};
