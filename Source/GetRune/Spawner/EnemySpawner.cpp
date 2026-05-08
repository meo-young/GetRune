#include "EnemySpawner.h"
#include "GetRune/GetRune.h"
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
	
	// 플레이어 스테이트에 대한 참조를 받아옵니다.
	const AGRPlayerState* PlayerState = Cast<AGRPlayerState>(Player->GetPlayerState());
	if (!PlayerState) return;
	
	// 데이터 테이블에서 현재 스테이지 정보를 받아옵니다.
	const UGRDataTableSubsystem* DTS = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGRDataTableSubsystem>();
	CurrentStageInfo = DTS->GetStageInfo(PlayerState->GetCurrentStageNum());
}

void UEnemySpawner::Spawn()
{
	Super::Spawn();
}

FVector UEnemySpawner::GetRandomSpawnLocation() const
{
	return Super::GetRandomSpawnLocation();
}
