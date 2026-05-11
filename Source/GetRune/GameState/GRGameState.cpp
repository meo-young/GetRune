#include "GRGameState.h"
#include "GetRune/Spawner/EnemySpawner.h"
#include "GetRune/Spawner/RuneSpawner.h"

void AGRGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	CreateSpawners();
}

void AGRGameState::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeSpawners();
	StartSpawners();
}

void AGRGameState::CreateSpawners()
{
	EnemySpawnManager = NewObject<UEnemySpawner>(this);
	RuneSpawnManager = NewObject<URuneSpawner>(this);
}

void AGRGameState::InitializeSpawners()
{
	RuneSpawnManager->Initialize();
	EnemySpawnManager->Initialize();
}

void AGRGameState::StartSpawners()
{
	RuneSpawnManager->StartSpawn();
	EnemySpawnManager->StartSpawn();
}
