#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_EnemyBase.generated.h"

class UAbilitySystemComponent;
class AGREnemy;
class AGRAIController;

/** GetContext() 호출 결과를 담는 구조체입니다. */
struct FEnemyTaskContext
{
	AGREnemy* Enemy = nullptr;
	AGRAIController* AIController = nullptr;
	UBlackboardComponent* BB = nullptr;
	UAbilitySystemComponent* ASC = nullptr;

	/** Enemy와 AIController가 모두 유효한지 반환합니다. */
	bool IsValid() const { return Enemy != nullptr && AIController != nullptr && BB != nullptr && ASC != nullptr; }
};

UCLASS()
class GETRUNE_API UBTTask_EnemyBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
// Lifecycle	
public:
	UBTTask_EnemyBase();
	
	
// Member Function
protected:
	/** OwnerComp에서 Enemy, AIController, BB, ASC를 한 번에 수집하여 반환합니다. */
	FEnemyTaskContext GetContext(UBehaviorTreeComponent& OwnerComp) const;
	
};
