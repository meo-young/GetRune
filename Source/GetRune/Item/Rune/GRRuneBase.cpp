#include "GRRuneBase.h"

#include "GetRune/GameMode/GRGameMode.h"
#include "GetRune/Spawner/RuneSpawner.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"

void AGRRuneBase::BeginPlay()
{
	Super::BeginPlay();
	
	RuneSpawner = Cast<AGRGameMode>(GetWorld()->GetAuthGameMode())->RuneSpawnManager;	
}

void AGRRuneBase::OnPlayerOverlapped()
{
	Super::OnPlayerOverlapped();
	
	--RuneSpawner->SpawnedCount;
}
