#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "GetRune/AI/GRAIHelper.h"
#include "BTDecorator_EnemyBase.generated.h"

UCLASS()
class GETRUNE_API UBTDecorator_EnemyBase : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
// Lifecycle	
public:
	UBTDecorator_EnemyBase();

};
