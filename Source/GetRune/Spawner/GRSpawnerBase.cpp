#include "GRSpawnerBase.h"
#include "GameFramework/Volume.h"
#include "GetRune/Player/GRPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GetRune/Subsystem/GRDataTableSubsystem.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"

const FName UGRSpawnerBase::SpawnZoneTag = FName("RuneSpawnZone");

void UGRSpawnerBase::Initialize()
{
	Player = Cast<AGRPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player) return;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), SpawnZoneTag, FoundActors);
	if (FoundActors.Num() > 0)
	{
		SpawnZoneVolume = Cast<AVolume>(FoundActors[0]);
		if (SpawnZoneVolume.IsValid())
		{
			SpawnZoneBounds = SpawnZoneVolume->GetComponentsBoundingBox(true);
		}
	}
}

FVector UGRSpawnerBase::FindSpawnLocationInZone(float MinRadius, float MaxRadius) const
{
	auto GenerateCandidate = [&]() -> FVector
	{
		const float Angle = FMath::RandRange(0.f, 360.f);
		const float Radius = FMath::RandRange(MinRadius, MaxRadius);
		const FVector Dir(
			FMath::Cos(FMath::DegreesToRadians(Angle)),
			FMath::Sin(FMath::DegreesToRadians(Angle)),
			0.f
		);
		return Player->GetActorLocation() + Dir * Radius;
	};

	auto IsLocationClear = [&](const FVector& Location) -> bool
	{
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Player);
		return !GetWorld()->OverlapAnyTestByChannel(
			Location,
			FQuat::Identity,
			ECC_WorldStatic,
			FCollisionShape::MakeSphere(SpawnCheckRadius),
			QueryParams
		);
	};

	if (!SpawnZoneBounds.IsValid)
	{
		const FVector Candidate = GenerateCandidate();
		return IsLocationClear(Candidate) ? Candidate : Player->GetActorLocation();
	}

	for (int32 i = 0; i < MaxSpawnAttempts; ++i)
	{
		const FVector Candidate = GenerateCandidate();
		if (SpawnZoneBounds.IsInsideOrOn(Candidate) && IsLocationClear(Candidate))
		{
			return Candidate;
		}
	}

	return Player->GetActorLocation();
}

void UGRSpawnerBase::SpawnMultiple(uint8 Count)
{
	// 지정한 수만큼 룬을 한 번에 소환합니다.
	for (int i = 0; i < Count; i++)
	{
		Spawn();
	}
}

void UGRSpawnerBase::StartSpawn()
{
	// 룬 소환 타이머를 가동합니다.
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::Spawn, SpawnInterval, true);
}

void UGRSpawnerBase::StopSpawn()
{
	// 룬 소환 타이머를 중단합니다.
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

UGRObjectPoolSubsystem* UGRSpawnerBase::GetObjectPoolSubsystem() const
{
	return UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGRObjectPoolSubsystem>();
}

UGRDataTableSubsystem* UGRSpawnerBase::GetDataTableSubsystem() const
{
	return UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGRDataTableSubsystem>();
}
