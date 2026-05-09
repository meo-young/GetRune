#include "AnimNotify_FireProjectile.h"
#include "GetRune/Enemy/GRRangedEnemy.h"

void UAnimNotify_FireProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AGRRangedEnemy* Enemy = Cast<AGRRangedEnemy>(MeshComp->GetOwner());
	if (Enemy)
	{
		Enemy->FireProjectile();
	}
}
