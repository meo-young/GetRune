#include "EnemySpawner.h"
#include "GetRune/GetRune.h"
#include "GetRune/Enemy/GREnemy.h"
#include "GetRune/Player/GRPlayer.h"
#include "GetRune/Player/GRPlayerState.h"
#include "GetRune/Subsystem/GRDataTableSubsystem.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"

UEnemySpawner::UEnemySpawner()
{
}

void UEnemySpawner::Initialize()
{
	Super::Initialize();

	const AGRPlayerState* PlayerState = Cast<AGRPlayerState>(Player->GetPlayerState());
	if (!PlayerState) return;

	const UGRDataTableSubsystem* DTS = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGRDataTableSubsystem>();
	CurrentStageInfo = DTS->GetStageInfo(PlayerState->GetCurrentStageNum());
}

void UEnemySpawner::StartSpawn()
{
	CurrentWaveNum = 0;
	StartWave();
}

void UEnemySpawner::StartWave()
{
	if (!CurrentStageInfo || CurrentWaveNum >= CurrentStageInfo->WaveInfos.Num()) return;

	OnWaveChanged.Broadcast(CurrentWaveNum + 1);

	const FWaveInfo& WaveInfo = CurrentStageInfo->WaveInfos[CurrentWaveNum];
	SpawnInterval = WaveInfo.EnemySpawnInterval;
	Super::StartSpawn();

	if (CurrentWaveNum == CurrentStageInfo->WaveInfos.Num() - 1)
	{
		SpawnBoss();
	}

	GetWorld()->GetTimerManager().SetTimer(
		WaveTimerHandle, this, &ThisClass::OnWaveEnd,
		static_cast<float>(WaveInfo.WaveDuration), false
	);
}

void UEnemySpawner::OnWaveEnd()
{
	StopSpawn();
	CurrentWaveNum++;

	if (CurrentWaveNum < CurrentStageInfo->WaveInfos.Num())
	{
		StartWave();
	}
}

void UEnemySpawner::Spawn()
{
	Super::Spawn();

	if (!CurrentStageInfo || CurrentWaveNum >= CurrentStageInfo->WaveInfos.Num()) return;

	const UEnemyInfo* EnemyInfo = SelectEnemyByWeight(CurrentStageInfo->WaveInfos[CurrentWaveNum]);
	if (!EnemyInfo || !EnemyInfo->EnemyClass) return;

	AGREnemy* Enemy = Cast<AGREnemy>(GetObjectPoolSubsystem()->AcquireActor(EnemyInfo->EnemyClass, GetRandomSpawnLocation(), FRotator::ZeroRotator));
	if (!Enemy) return;

	Enemy->InitializeFromEnemyInfo(EnemyInfo);
	++CurrentEnemyNum;
	OnEnemyCountChanged.Broadcast(CurrentEnemyNum);
}

void UEnemySpawner::SpawnBoss()
{
	if (!CurrentStageInfo || !CurrentStageInfo->BossInfo || !CurrentStageInfo->BossInfo->EnemyClass) return;

	const UEnemyInfo* BossInfo = CurrentStageInfo->BossInfo;
	AGREnemy* Boss = Cast<AGREnemy>(GetObjectPoolSubsystem()->AcquireActor(BossInfo->EnemyClass, GetRandomSpawnLocation(), FRotator::ZeroRotator));
	if (!Boss) return;

	Boss->InitializeFromEnemyInfo(BossInfo);
}

const UEnemyInfo* UEnemySpawner::SelectEnemyByWeight(const FWaveInfo& WaveInfo) const
{
	int32 TotalWeight = 0;
	for (const TPair<TObjectPtr<UEnemyInfo>, int32>& Pair : WaveInfo.EnemyClasses)
	{
		TotalWeight += Pair.Value;
	}
	if (TotalWeight <= 0) return nullptr;

	const int32 RandValue = FMath::RandRange(0, TotalWeight - 1);
	int32 Accumulated = 0;
	for (const TPair<TObjectPtr<UEnemyInfo>, int32>& Pair : WaveInfo.EnemyClasses)
	{
		Accumulated += Pair.Value;
		if (RandValue < Accumulated)
		{
			return Pair.Key.Get();
		}
	}
	return nullptr;
}

void UEnemySpawner::DecrementEnemyNum()
{
	--CurrentEnemyNum;
	OnEnemyCountChanged.Broadcast(CurrentEnemyNum);
}

FVector UEnemySpawner::GetRandomSpawnLocation() const
{
	const float Angle = FMath::RandRange(0.f, 360.f);
	const FVector Dir(
		FMath::Cos(FMath::DegreesToRadians(Angle)),
		FMath::Sin(FMath::DegreesToRadians(Angle)),
		0.f
	);
	return Player->GetActorLocation() + Dir * 1400.f;
}
