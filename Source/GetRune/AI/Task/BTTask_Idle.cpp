#include "BTTask_Idle.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GetRune/Enemy/GREnemy.h"
#include "GetRune/Player/GRPlayer.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Idle::UBTTask_Idle()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTTask_Idle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	const FEnemyTaskContext Ctx = GetContext(OwnerComp);
	if (!Ctx.IsValid()) return;

	const AGRPlayer* Player = Cast<AGRPlayer>(UGameplayStatics::GetPlayerCharacter(OwnerComp.GetWorld(), 0));
	if (!Player) return;

	const FVector EnemyLoc  = Ctx.Enemy->GetActorLocation();
	const FVector PlayerLoc = Player->GetActorLocation();
	const float   Dist      = FVector::Dist2D(EnemyLoc, PlayerLoc);

	if (Dist > Ctx.Enemy->GetAttackRange())
	{
		const FVector Direction = (PlayerLoc - EnemyLoc).GetSafeNormal2D();
		Ctx.Enemy->AddMovementInput(Direction);
	}
	else
	{
		Ctx.Enemy->GetCharacterMovement()->StopMovementImmediately();
	}
}
