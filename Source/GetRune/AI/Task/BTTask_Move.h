#pragma once

#include "CoreMinimal.h"
#include "BTTask_EnemyBase.h"
#include "BTTask_Move.generated.h"

UCLASS()
class GETRUNE_API UBTTask_Move : public UBTTask_EnemyBase
{
	GENERATED_BODY()
	
public:
	UBTTask_Move();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};
