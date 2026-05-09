#pragma once

#include "CoreMinimal.h"

class UAbilitySystemComponent;
class AGREnemy;
class AGRAIController;
class UBlackboardComponent;
class UBehaviorTreeComponent;

struct FEnemyTaskContext
{
	AGREnemy* Enemy = nullptr;
	AGRAIController* AIController = nullptr;
	UBlackboardComponent* BB = nullptr;
	UAbilitySystemComponent* ASC = nullptr;
	float TargetAccumulatedTime = 0.0f;

	bool IsValid() const { return Enemy != nullptr && AIController != nullptr && BB != nullptr && ASC != nullptr; }
};

class FGRAIHelper
{
public:
	static FEnemyTaskContext GetContext(UBehaviorTreeComponent& OwnerComp);
};
