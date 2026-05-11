#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GRGameState.generated.h"

class URuneSpawner;
class UEnemySpawner;

UCLASS()
class GETRUNE_API AGRGameState : public AGameStateBase
{
	GENERATED_BODY()

// Lifecycle	
public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	
// Member Function	
private:
	void CreateSpawners();
	void InitializeSpawners();
	void StartSpawners();

	
// Member Variable	
public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UEnemySpawner> EnemySpawnManager;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<URuneSpawner> RuneSpawnManager;
};
