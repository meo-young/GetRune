#include "GRAIController.h"

AGRAIController::AGRAIController()
{
}

void AGRAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}
