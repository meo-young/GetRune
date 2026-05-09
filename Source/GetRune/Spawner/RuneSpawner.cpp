#include "RuneSpawner.h"
#include "GameFramework/Character.h"
#include "GetRune/GetRune.h"
#include "GetRune/Data/RuneInfo.h"
#include "GetRune/Item/Rune/GRRuneBase.h"
#include "GetRune/Player/GRPlayer.h"
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
	
	// 데이터 테이블에서 소유 캐릭터의 룬 설정을 가져옵니다.
	const UGRDataTableSubsystem* DTS = GetDataTableSubsystem();
	if (const FCharacterInfo* Info = DTS->GetCharacterInfo(Player->GetClass()))
	{
		AllowedRuneTypes = Info->GetAllowedRuneTypes();
		SpawnInterval = Info->RuneSpawnInterval;
	}

	// 허용된 룬 타입별로 오브젝트 풀을 미리 생성합니다.
	if (AllowedRuneTypes.Num() > 0)
	{
		UGRObjectPoolSubsystem* Pool = GetObjectPoolSubsystem();

		for (ERuneType RuneType : AllowedRuneTypes)
		{
			const int32 RunePerMaxSize = RuneData->MaxRuneCount/AllowedRuneTypes.Num();
			const TSubclassOf<AGRRuneBase>* RuneClass = RuneData->RuneClass.Find(RuneType);
			
			Pool->InitializePools(*RuneClass, RunePerMaxSize);
		}
	}

	SpawnedCount = 0;
	StartSpawn();
}

void URuneSpawner::Spawn()
{
	if (AllowedRuneTypes.IsEmpty()) return;
	
	if (SpawnedCount >= RuneData->MaxRuneCount) return;
	
	UGRObjectPoolSubsystem* Pool = GetObjectPoolSubsystem();
	if (!Pool) return;

	// 허용된 타입 중 하나를 무작위로 선택합니다.
	const ERuneType RuneType = AllowedRuneTypes[FMath::RandRange(0, AllowedRuneTypes.Num() - 1)];

	// 선택한 타입에 해당하는 룬 클래스를 조회합니다.
	const TSubclassOf<AGRRuneBase>* RuneClass = RuneData->RuneClass.Find(RuneType);
	if (!RuneClass || !*RuneClass) return;

	// 풀에서 룬을 꺼내 무작위 위치에 배치합니다.
	Pool->AcquireActor(*RuneClass, GetRandomSpawnLocation(), FRotator::ZeroRotator);
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
