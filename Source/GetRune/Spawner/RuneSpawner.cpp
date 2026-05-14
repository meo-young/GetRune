#include "RuneSpawner.h"
#include "GameFramework/Character.h"
#include "GetRune/GetRune.h"
#include "GetRune/Data/RuneInfo.h"
#include "GetRune/Data/StageInfo.h"
#include "GetRune/Item/Rune/GRRuneBase.h"
#include "GetRune/Player/GRPlayer.h"
#include "GetRune/Player/GRPlayerState.h"
#include "GetRune/Subsystem/GRDataTableSubsystem.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"

URuneSpawner::URuneSpawner()
{
	// 룬 정보 데이터 에셋을 에디터 의존 없이 불러옵니다.
	static ConstructorHelpers::FObjectFinder<URuneInfo> DA_RuneInfo(TEXT("/Game/_GetRune/DataTable/DA_RuneInfo.DA_RuneInfo"));
	if (DA_RuneInfo.Succeeded())
	{
		RuneData = DA_RuneInfo.Object;
	}
}

void URuneSpawner::Initialize()
{
	Super::Initialize();

	const UGRDataTableSubsystem* DTS = GetDataTableSubsystem();

	if (const FCharacterInfo* Info = DTS->GetCharacterInfo(Player->GetClass()))
	{
		AllowedRuneTypes = Info->GetAllowedRuneTypes();
	}

	const AGRPlayerState* PlayerState = Cast<AGRPlayerState>(Player->GetPlayerState());
	if (PlayerState)
	{
		CurrentStageInfo = DTS->GetStageInfo(PlayerState->GetCurrentStageNum());
	}

	// 전체 웨이브 중 가장 큰 MaxRuneCount로 풀을 미리 할당합니다.
	int32 PeakMaxRuneCount = 0;
	if (CurrentStageInfo)
	{
		for (const FWaveInfo& WaveInfo : CurrentStageInfo->WaveInfos)
		{
			PeakMaxRuneCount = FMath::Max(PeakMaxRuneCount, WaveInfo.MaxRuneCount);
		}
	}

	if (AllowedRuneTypes.Num() > 0 && PeakMaxRuneCount > 0)
	{
		UGRObjectPoolSubsystem* Pool = GetObjectPoolSubsystem();

		for (ERuneType RuneType : AllowedRuneTypes)
		{
			const int32 RunePerMaxSize = PeakMaxRuneCount / AllowedRuneTypes.Num();
			const FRuneClassData* RuneClassData = RuneData->RuneClass.Find(RuneType);
			Pool->InitializePools(RuneClassData->RuneClass, RunePerMaxSize);
		}
	}

	SpawnedCount = 0;
}

void URuneSpawner::OnWaveStarted(int32 WaveIndex)
{
	if (!CurrentStageInfo || !CurrentStageInfo->WaveInfos.IsValidIndex(WaveIndex)) return;

	const FWaveInfo& WaveInfo = CurrentStageInfo->WaveInfos[WaveIndex];
	StopSpawn();
	SpawnInterval = WaveInfo.RuneSpawnInterval;
	CurrentMaxRuneCount = WaveInfo.MaxRuneCount;
	SpawnedCount = 0;
	StartSpawn();
}

void URuneSpawner::Spawn()
{
	if (AllowedRuneTypes.IsEmpty()) return;
	
	if (SpawnedCount >= CurrentMaxRuneCount) return;
	
	UGRObjectPoolSubsystem* Pool = GetObjectPoolSubsystem();
	if (!Pool) return;

	// 허용된 타입 중 하나를 무작위로 선택합니다.
	const ERuneType RuneType = AllowedRuneTypes[FMath::RandRange(0, AllowedRuneTypes.Num() - 1)];

	// 선택한 타입에 해당하는 룬 클래스를 조회합니다.
	const FRuneClassData* RuneClassData = RuneData->RuneClass.Find(RuneType);
	if (!RuneClassData || !RuneClassData->RuneClass) return;

	// 풀에서 룬을 꺼내 무작위 위치에 배치합니다.
	Pool->AcquireActor(RuneClassData->RuneClass, GetRandomSpawnLocation(), FRotator::ZeroRotator);
	++SpawnedCount;
}

FVector URuneSpawner::GetRandomSpawnLocation() const
{
	// 무작위 각도와 반경으로 소환 위치를 계산합니다.
	const float Angle  = FMath::RandRange(0.f, 360.f);
	const float Radius = FMath::RandRange(RuneData->InnerRadius, RuneData->OuterRadius);
	const FVector Dir(
		FMath::Cos(FMath::DegreesToRadians(Angle)),
		FMath::Sin(FMath::DegreesToRadians(Angle)),
		0.f
	);

	// 소유자를 중심으로 원형 범위 내 임의 지점을 반환합니다.
	return Player->GetActorLocation() + Dir * Radius;
}
