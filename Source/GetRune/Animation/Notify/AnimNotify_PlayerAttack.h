#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayerAttack.generated.h"

class AGRProjectilePiercing;

UCLASS()
class GETRUNE_API UAnimNotify_PlayerAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category = "변수")
	TSubclassOf<AGRProjectilePiercing> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "변수", meta = (DisplayName = "적 탐색 반경"))
	float EnemySearchRadius = 2000.f;
};
