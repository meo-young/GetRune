#pragma once

#include "CoreMinimal.h"
#include "GRSpawnerBase.h"
#include "GetRune/Data/StageInfo.h"
#include "UObject/Object.h"
#include "EnemySpawner.generated.h"

class AGRPlayer;

UCLASS()
class GETRUNE_API UEnemySpawner : public UGRSpawnerBase
{
	GENERATED_BODY()
	
// Lifecycle	
public:
	UEnemySpawner();
	

// Member Function	
public:
	virtual void Initialize() override;
	virtual void Spawn() override;

private:
	virtual FVector GetRandomSpawnLocation() const override;
	
	
// Member Variable	
private:
	const FStageInfo* CurrentStageInfo;
	
};
