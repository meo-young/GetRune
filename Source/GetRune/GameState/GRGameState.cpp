#include "GRGameState.h"
#include "GetRune/Spawner/EnemySpawner.h"
#include "GetRune/Spawner/RuneSpawner.h"
#include "GetRune/Subsystem/SoundSubsystem.h"

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
	USoundSubsystem::Get(this).PlayBGMByName(TEXT("InGameBGM"));
	
	EnemySpawnManager->OnWaveChanged.AddDynamic(this, &ThisClass::OnWaveChanged);
	EnemySpawnManager->StartSpawn();
}

void AGRGameState::OnWaveChanged(int32 WaveNum)
{
	RuneSpawnManager->OnWaveStarted(WaveNum - 1);
}
