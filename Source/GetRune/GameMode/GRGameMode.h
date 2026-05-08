#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GRGameMode.generated.h"

class URuneSpawner;
class UEnemySpawner;

UCLASS()
class GETRUNE_API AGRGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
public:
	UPROPERTY()
	TObjectPtr<UEnemySpawner> EnemySpawnManager;
	
	UPROPERTY()
	TObjectPtr<URuneSpawner> RuneSpawnManager;
	
};
