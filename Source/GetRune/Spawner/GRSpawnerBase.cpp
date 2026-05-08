#include "GRSpawnerBase.h"
#include "GetRune/Player/GRPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GetRune/Subsystem/GRDataTableSubsystem.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"

void UGRSpawnerBase::Initialize()
{
	// 플레이어에 대한 참조를 받아옵니다.
	Player = Cast<AGRPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player) return;
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
