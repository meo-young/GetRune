#include "GRRuneBase.h"

#include "GetRune/GameMode/GRGameMode.h"
#include "GetRune/Player/GRPlayer.h"
#include "GetRune/Spawner/RuneSpawner.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"

void AGRRuneBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGRRuneBase::OnPlayerOverlapped()
{
	Super::OnPlayerOverlapped();
}
