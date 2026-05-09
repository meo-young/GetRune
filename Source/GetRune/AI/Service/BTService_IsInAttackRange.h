#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsInAttackRange.generated.h"

UCLASS()
class GETRUNE_API UBTService_IsInAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_IsInAttackRange();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
