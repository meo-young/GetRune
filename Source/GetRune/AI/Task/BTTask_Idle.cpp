#include "BTTask_Idle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GetRune/AI/Controller/GRAIController.h"
#include "GetRune/Enemy/GREnemy.h"

UBTTask_Idle::UBTTask_Idle()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FEnemyTaskContext* Ctx = CastInstanceNodeMemory<FEnemyTaskContext>(NodeMemory);
	*Ctx = FGRAIHelper::GetContext(OwnerComp);
	
	Ctx->AIController->StopMovement();

	return EBTNodeResult::InProgress;
}

void UBTTask_Idle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	const FEnemyTaskContext* Ctx = CastInstanceNodeMemory<FEnemyTaskContext>(NodeMemory);
	if (!Ctx->IsValid()) return;
	
	const bool CanAttack = Ctx->Enemy->CanAttack();
	Ctx->BB->SetValueAsBool(TEXT("CanAttack"), CanAttack);
	
}
