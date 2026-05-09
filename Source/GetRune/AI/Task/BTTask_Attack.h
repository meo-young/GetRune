#pragma once

#include "CoreMinimal.h"
#include "BTTask_EnemyBase.h"
#include "BTTask_Attack.generated.h"

UCLASS()
class GETRUNE_API UBTTask_Attack : public UBTTask_EnemyBase
{
	GENERATED_BODY()

public:
	UBTTask_Attack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	FTimerHandle AttackMontageTimerHandle;

};
