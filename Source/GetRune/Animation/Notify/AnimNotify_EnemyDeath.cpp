#include "AnimNotify_EnemyDeath.h"
#include "GetRune/Enemy/GREnemy.h"

void UAnimNotify_EnemyDeath::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AGREnemy* Enemy = Cast<AGREnemy>(MeshComp->GetOwner()))
	{
		Enemy->FinishDeath();
	}
}
