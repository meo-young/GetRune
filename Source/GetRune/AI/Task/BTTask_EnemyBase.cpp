#include "BTTask_EnemyBase.h"
#include "GetRune/AI/Controller/GRAIController.h"
#include "GetRune/Enemy/GREnemy.h"

UBTTask_EnemyBase::UBTTask_EnemyBase()
{
	bCreateNodeInstance = false;
}

FEnemyTaskContext UBTTask_EnemyBase::GetContext(UBehaviorTreeComponent& OwnerComp) const
{
	FEnemyTaskContext Ctx;

	Ctx.AIController = Cast<AGRAIController>(OwnerComp.GetAIOwner());
	if (!Ctx.AIController) return Ctx;

	Ctx.Enemy = Cast<AGREnemy>(Ctx.AIController->GetPawn());
	Ctx.BB    = OwnerComp.GetBlackboardComponent();
	Ctx.ASC   = Ctx.Enemy->GetAbilitySystemComponent();

	return Ctx;
}
