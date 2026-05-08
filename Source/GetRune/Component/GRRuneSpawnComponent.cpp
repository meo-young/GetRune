#include "GRRuneSpawnComponent.h"
#include "GetRune/GetRune.h"
#include "GetRune/Data/RuneInfo.h"
#include "GetRune/Item/Rune/GRRuneBase.h"
#include "GetRune/Subsystem/GRDataTableSubsystem.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"

UGRRuneSpawnComponent::UGRRuneSpawnComponent()
{
	static ConstructorHelpers::FObjectFinder<URuneInfo> DA_RuneInfo(TEXT("/Game/_GetRune/DataTable/DA_RuneInfo.DA_RuneInfo"));
	if (DA_RuneInfo.Succeeded())
	{
		RuneData = DA_RuneInfo.Object;
	}
}

void UGRRuneSpawnComponent::BeginPlay()
{
	Super::BeginPlay();
	
	const UGRDataTableSubsystem* DTS = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGRDataTableSubsystem>();
	if (const FCharacterInfo* Info = DTS->GetCharacterInfo(GetOwner()->GetClass()))
	{
		AllowedRuneTypes = Info->GetAllowedRuneTypes();
		RuneSpawnInterval = Info->RuneSpawnInterval;
		StartRuneSpawn();
	}
}

void UGRRuneSpawnComponent::SpawnRune()
{
	if (AllowedRuneTypes.IsEmpty()) return;

	UGRObjectPoolSubsystem* Pool = GetOwner()->GetGameInstance()->GetSubsystem<UGRObjectPoolSubsystem>();
	if (!Pool) return;

	const ERuneType RuneType = AllowedRuneTypes[FMath::RandRange(0, AllowedRuneTypes.Num() - 1)];

	const TSubclassOf<AGRRuneBase>* RuneClass = RuneData->RuneClass.Find(RuneType);
	if (!RuneClass || !*RuneClass) return;

	if (AGRRuneBase* Rune = Cast<AGRRuneBase>(Pool->AcquireActor(*RuneClass, GetRandomSpawnLocation(), FRotator::ZeroRotator)))
	{
		Rune->RuneType = RuneType;
	}
}

void UGRRuneSpawnComponent::SpawnRunes(uint8 Count)
{
	for (int i = 0; i < Count; i++)
	{
		SpawnRune();
	}
}

void UGRRuneSpawnComponent::StartRuneSpawn()
{
	GetWorld()->GetTimerManager().SetTimer(RuneSpawnTimerHandle, this, &ThisClass::SpawnRune, RuneSpawnInterval, true);
}

void UGRRuneSpawnComponent::StopRuneSpawn()
{
	GetWorld()->GetTimerManager().ClearTimer(RuneSpawnTimerHandle);
}

FVector UGRRuneSpawnComponent::GetRandomSpawnLocation() const
{
	const float Angle  = FMath::RandRange(0.f, 360.f);
	const float Radius = FMath::RandRange(InnerRadius, OuterRadius);
	const FVector Dir(
		FMath::Cos(FMath::DegreesToRadians(Angle)),
		FMath::Sin(FMath::DegreesToRadians(Angle)),
		0.f
	);
	return GetOwner()->GetActorLocation() + Dir * Radius;
}
