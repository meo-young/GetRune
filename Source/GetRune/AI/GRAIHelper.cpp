#include "GRAIHelper.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GetRune/AI/Controller/GRAIController.h"
#include "GetRune/Enemy/GREnemy.h"

FEnemyTaskContext FGRAIHelper::GetContext(UBehaviorTreeComponent& OwnerComp)
{
	FEnemyTaskContext Ctx;

	Ctx.AIController = Cast<AGRAIController>(OwnerComp.GetAIOwner());
	if (!Ctx.AIController) return Ctx;

	Ctx.Enemy = Cast<AGREnemy>(Ctx.AIController->GetPawn());
	Ctx.BB    = OwnerComp.GetBlackboardComponent();
	Ctx.ASC   = Ctx.Enemy->GetAbilitySystemComponent();

	return Ctx;
}
