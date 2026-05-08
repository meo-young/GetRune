#pragma once

#include "CoreMinimal.h"
#include "StageInfo.generated.h"

class AGRItemBase;
class AGREnemy;

UCLASS()
class GETRUNE_API UEnemyInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGREnemy> EnemyClass;
	
	UPROPERTY(EditAnywhere)
	int32 MoveSpeed;
	
	UPROPERTY(EditAnywhere)
	int32 Health;
	
	UPROPERTY(EditAnywhere)
	int32 AttackPower;
	
	UPROPERTY(EditAnywhere)
	TMap<float, TSubclassOf<AGRItemBase>> DroppedItems;
	
	UPROPERTY(EditAnywhere)
	float AttackCooldown;
	
};

USTRUCT()
struct FWaveInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TMap<int32, TObjectPtr<UEnemyInfo>> EnemyClasses;
	
	UPROPERTY(EditAnywhere)
	int32 WaveDuration = 30;
	
	UPROPERTY(EditAnywhere)
	float EnemySpawnInterval = 2;
	
	UPROPERTY(EditAnywhere)
	int32 BonusRuneCount = 5;
	
};

USTRUCT()
struct GETRUNE_API FStageInfo : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FWaveInfo> WaveInfos;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UEnemyInfo> BossInfo;


};
