#include "GRRuneBase.h"

#include "GetRune/GameMode/GRGameMode.h"
#include "GetRune/Player/GRPlayer.h"
#include "GetRune/Spawner/RuneSpawner.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"

void AGRRuneBase::BeginPlay()
{
	Super::BeginPlay();
	
	RuneSpawner = Cast<AGRGameMode>(GetWorld()->GetAuthGameMode())->RuneSpawnManager;	
}

void AGRRuneBase::OnPlayerOverlapped()
{
	if (AGRPlayer* Player = Cast<AGRPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		Player->AddRune(RuneType);
	}

	Super::OnPlayerOverlapped();

	--RuneSpawner->SpawnedCount;
}
