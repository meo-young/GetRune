#include "AnimNotify_RestartLevel.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_RestartLevel::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (UWorld* World = MeshComp->GetWorld())
	{
		UGameplayStatics::OpenLevel(World, FName(*World->GetName()));
	}
}
