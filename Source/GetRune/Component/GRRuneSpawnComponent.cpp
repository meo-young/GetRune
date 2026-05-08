#include "GRRuneSpawnComponent.h"
#include "GetRune/GetRune.h"
#include "GetRune/Data/RuneInfo.h"
#include "GetRune/Item/Rune/GRRuneBase.h"
#include "GetRune/Subsystem/GRDataTableSubsystem.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"

UGRRuneSpawnComponent::UGRRuneSpawnComponent()
{
	// 룬 정보 데이터 에셋을 에디터 의존 없이 불러옵니다.
	static ConstructorHelpers::FObjectFinder<URuneInfo> DA_RuneInfo(TEXT("/Game/_GetRune/DataTable/DA_RuneInfo.DA_RuneInfo"));
	if (DA_RuneInfo.Succeeded())
	{
		RuneData = DA_RuneInfo.Object;
	}
}

void UGRRuneSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	// 데이터 테이블에서 소유 캐릭터의 룬 설정을 가져옵니다.
	const UGRDataTableSubsystem* DTS = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGRDataTableSubsystem>();
	if (const FCharacterInfo* Info = DTS->GetCharacterInfo(GetOwner()->GetClass()))
	{
		AllowedRuneTypes = Info->GetAllowedRuneTypes();
		RuneSpawnInterval = Info->RuneSpawnInterval;
	}

	// 허용된 룬 타입별로 오브젝트 풀을 미리 생성합니다.
	if (AllowedRuneTypes.Num() > 0)
	{
		UGRObjectPoolSubsystem* Pool = GetOwner()->GetGameInstance()->GetSubsystem<UGRObjectPoolSubsystem>();

		for (ERuneType RuneType : AllowedRuneTypes)
		{
			Pool->InitializePools(*RuneData->RuneClass.Find(RuneType), 50);
		}
	}

	StartRuneSpawn();
}

void UGRRuneSpawnComponent::SpawnRune()
{
	if (AllowedRuneTypes.IsEmpty()) return;

	UGRObjectPoolSubsystem* Pool = GetOwner()->GetGameInstance()->GetSubsystem<UGRObjectPoolSubsystem>();
	if (!Pool) return;

	// 허용된 타입 중 하나를 무작위로 선택합니다.
	const ERuneType RuneType = AllowedRuneTypes[FMath::RandRange(0, AllowedRuneTypes.Num() - 1)];

	// 선택한 타입에 해당하는 룬 클래스를 조회합니다.
	const TSubclassOf<AGRRuneBase>* RuneClass = RuneData->RuneClass.Find(RuneType);
	if (!RuneClass || !*RuneClass) return;

	// 풀에서 룬을 꺼내 무작위 위치에 배치합니다.
	Pool->AcquireActor(*RuneClass, GetRandomSpawnLocation(), FRotator::ZeroRotator);
}

void UGRRuneSpawnComponent::SpawnRunes(uint8 Count)
{
	// 지정한 수만큼 룬을 한 번에 소환합니다.
	for (int i = 0; i < Count; i++)
	{
		SpawnRune();
	}
}

void UGRRuneSpawnComponent::StartRuneSpawn()
{
	// 일정 간격으로 SpawnRune을 반복 호출하는 타이머를 시작합니다.
	GetWorld()->GetTimerManager().SetTimer(RuneSpawnTimerHandle, this, &ThisClass::SpawnRune, RuneSpawnInterval, true);
}

void UGRRuneSpawnComponent::StopRuneSpawn()
{
	// 룬 소환 타이머를 중단합니다.
	GetWorld()->GetTimerManager().ClearTimer(RuneSpawnTimerHandle);
}

FVector UGRRuneSpawnComponent::GetRandomSpawnLocation() const
{
	// 무작위 각도와 반경으로 소환 위치를 계산합니다.
	const float Angle  = FMath::RandRange(0.f, 360.f);
	const float Radius = FMath::RandRange(InnerRadius, OuterRadius);
	const FVector Dir(
		FMath::Cos(FMath::DegreesToRadians(Angle)),
		FMath::Sin(FMath::DegreesToRadians(Angle)),
		0.f
	);

	// 소유자를 중심으로 원형 범위 내 임의 지점을 반환합니다.
	return GetOwner()->GetActorLocation() + Dir * Radius;
}
