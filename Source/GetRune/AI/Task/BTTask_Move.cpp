#include "BTTask_Move.h"
#include "GetRune/AI/Controller/GRAIController.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Move::UBTTask_Move()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Move::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FEnemyTaskContext* Ctx = CastInstanceNodeMemory<FEnemyTaskContext>(NodeMemory);
	*Ctx = FGRAIHelper::GetContext(OwnerComp);
	
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	Ctx->AIController->MoveToActor(Player, 0.f, false);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Move::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTTask_Move::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	
	const FEnemyTaskContext* Ctx = CastInstanceNodeMemory<FEnemyTaskContext>(NodeMemory);
	if (!Ctx->IsValid()) return;
	
	Ctx->AIController->StopMovement();
}
