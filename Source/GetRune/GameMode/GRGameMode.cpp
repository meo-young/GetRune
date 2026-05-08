#include "GRGameMode.h"
#include "GetRune/Spawner/EnemySpawner.h"
#include "GetRune/Spawner/RuneSpawner.h"

void AGRGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	EnemySpawnManager = NewObject<UEnemySpawner>(this);
	RuneSpawnManager = NewObject<URuneSpawner>(this);
}

void AGRGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Super 호출에서 RestartPlayer()가 실행되어 Pawn이 스폰 및 Possess됩니다.
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	// Pawn이 보장된 시점에 초기화합니다.
	RuneSpawnManager->Initialize();
}

void AGRGameMode::StartPlay()
{
	Super::StartPlay();
	
	RuneSpawnManager->StartRuneSpawn();
}
