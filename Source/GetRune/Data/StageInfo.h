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
	int32 MoveSpeed = 300.0f;
	
	UPROPERTY(EditAnywhere)
	int32 Health = 100.0f;
	
	UPROPERTY(EditAnywhere)
	int32 ContactDamage = 10.0f;
	
	UPROPERTY(EditAnywhere)
	int32 AttackPower = 0.0f;
	
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AGRItemBase>, float> DroppedItems;

	UPROPERTY(EditAnywhere)
	float AttackRange = 80.f;

	UPROPERTY(EditAnywhere)
	float AttackCooldown;
	
};

USTRUCT()
struct FWaveInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TMap<TObjectPtr<UEnemyInfo>, int32> EnemyClasses;
	
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
