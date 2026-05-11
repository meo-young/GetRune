#pragma once

#include "CoreMinimal.h"
#include "GRSpawnerBase.h"
#include "GetRune/Data/StageInfo.h"
#include "UObject/Object.h"
#include "EnemySpawner.generated.h"

class AGRPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyCountChanged, int32, EnemyCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveChanged, int32, WaveNum);

UCLASS()
class GETRUNE_API UEnemySpawner : public UGRSpawnerBase
{
	GENERATED_BODY()
	
// Lifecycle	
public:
	UEnemySpawner();
	
	
// Delegate	
public:
	FOnEnemyCountChanged OnEnemyCountChanged;

	FOnWaveChanged OnWaveChanged;
	

// Member Function
public:
	virtual void Initialize() override;
	virtual void StartSpawn() override;
	virtual void Spawn() override;

private:
	virtual FVector GetRandomSpawnLocation() const override;
	void StartWave();
	void OnWaveEnd();
	void SpawnBoss();
	const UEnemyInfo* SelectEnemyByWeight(const FWaveInfo& WaveInfo) const;


// Member Variable
private:
	const FStageInfo* CurrentStageInfo = nullptr;
	int32 CurrentWaveNum = 0;
	FTimerHandle WaveTimerHandle;
	int32 CurrentEnemyNum = 0;
	
	
// Getter, Setter	
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentEnemyNum() const { return CurrentEnemyNum; }
	
	void DecrementEnemyNum();

};
