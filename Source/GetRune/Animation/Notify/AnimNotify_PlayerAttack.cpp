#include "AnimNotify_PlayerAttack.h"
#include "GetRune/Player/GRPlayer.h"

void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AGRPlayer* Player = Cast<AGRPlayer>(MeshComp->GetOwner()))
	{
		Player->LaunchProjectile();
	}
}
