#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GetRune/AI/GRAIHelper.h"
#include "BTTask_EnemyBase.generated.h"

UCLASS()
class GETRUNE_API UBTTask_EnemyBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
// Lifecycle	
public:
	UBTTask_EnemyBase();
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FEnemyTaskContext); }
	
};
