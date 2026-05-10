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
	UPROPERTY(EditAnywhere, meta = (DisplayName = "적 종류"))
	TSubclassOf<AGREnemy> EnemyClass;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "이동속도"))
	int32 MoveSpeed = 300.0f;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "체력"))
	int32 Health = 100.0f;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "접촉 데미지"))
	float ContactDamage = 10.0f;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "공격 데미지"))
	float AttackPower = 0.0f;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "드랍 아이템"))
	TMap<TSubclassOf<AGRItemBase>, float> DroppedItems;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "보상 골드량"))
	int32 DroppedGolds = 1;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "공격 범위"))
	float AttackRange = 80.f;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "공격 간격"))
	float AttackCooldown;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "투사체 속도"))
	float ProjectileSpeed = 600.f;
	
};

USTRUCT()
struct FWaveInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TMap<TObjectPtr<UEnemyInfo>, int32> EnemyClasses;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "웨이브 지속시간"))
	int32 WaveDuration = 30;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "적 소환 간격"))
	float EnemySpawnInterval = 2;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "웨이브 종료 보상 룬 개수"))
	int32 BonusRuneCount = 5;
	
};

USTRUCT()
struct GETRUNE_API FStageInfo : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (DisplayName = "웨이브 정보"))
	TArray<FWaveInfo> WaveInfos;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "보스 종류"))
	TObjectPtr<UEnemyInfo> BossInfo;


};
