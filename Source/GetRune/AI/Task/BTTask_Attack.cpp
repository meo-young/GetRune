#include "BTTask_Attack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GetRune/GetRune.h"
#include "GetRune/AI/GRAIHelper.h"
#include "GetRune/AI/Controller/GRAIController.h"
#include "GetRune/Enemy/GRRangedEnemy.h"

UBTTask_Attack::UBTTask_Attack()
{
	bCreateNodeInstance = false;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FEnemyTaskContext* Ctx = CastInstanceNodeMemory<FEnemyTaskContext>(NodeMemory);
	*Ctx = FGRAIHelper::GetContext(OwnerComp);
	
	Ctx->BB->SetValueAsBool(TEXT("IsAttacking"), true);
	
	Ctx->AIController->StopMovement();
	Ctx->Enemy->Attack();
	
	const UAnimInstance* AnimInstance = Ctx->Enemy->GetMesh()->GetAnimInstance();
	Ctx->TargetAccumulatedTime = AnimInstance->GetCurrentActiveMontage()->GetPlayLength();
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	FEnemyTaskContext* Ctx = CastInstanceNodeMemory<FEnemyTaskContext>(NodeMemory);
	if (!Ctx->IsValid()) return;
	
	if (Ctx->TargetAccumulatedTime < 0)
	{
		Ctx->TargetAccumulatedTime = 0;
		Ctx->Enemy->OnAttackFinished();
		Ctx->BB->SetValueAsBool(TEXT("CanAttack"), false);
		Ctx->BB->SetValueAsBool(TEXT("IsAttacking"), false);
	}
	else
	{
		Ctx->TargetAccumulatedTime -= DeltaSeconds;
	}
}