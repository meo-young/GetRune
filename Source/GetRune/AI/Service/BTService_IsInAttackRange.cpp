#include "BTService_IsInAttackRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GetRune/AI/GRAIHelper.h"
#include "GetRune/Enemy/GREnemy.h"
#include "Kismet/GameplayStatics.h"

UBTService_IsInAttackRange::UBTService_IsInAttackRange()
{
	Interval = 0.5f;
	RandomDeviation = 0.0f;
}

void UBTService_IsInAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	const FEnemyTaskContext Ctx = FGRAIHelper::GetContext(OwnerComp);
	if (!Ctx.IsValid()) return;
	
	const APawn* Player = UGameplayStatics::GetPlayerPawn(OwnerComp.GetWorld(), 0);
	if (!Player) return;

	const FVector EnemyLoc  = Ctx.Enemy->GetActorLocation();
	const FVector PlayerLoc = Player->GetActorLocation();
	const float   Dist      = FVector::Dist2D(EnemyLoc, PlayerLoc);

	bool IsInAttackRange;
	if (Dist > Ctx.Enemy->GetAttackRange())
	{
		IsInAttackRange = false;
	}
	else
	{
		IsInAttackRange = true;
	}
	
	Ctx.BB->SetValueAsBool(TEXT("IsInAttackRange"), IsInAttackRange);
}
