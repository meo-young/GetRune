#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GetRune/Data/CharacterInfo.h"
#include "RuneSpawner.generated.h"

class UGRDataTableSubsystem;
class UGRObjectPoolSubsystem;
class URuneInfo;
class AGRRuneBase;

UCLASS()
class GETRUNE_API URuneSpawner : public UObject
{
	GENERATED_BODY()

// Lifecycle	
public:
	URuneSpawner();
	

// Member Function	
public:
	void Initialize();
	void SpawnRune();
	void SpawnRunes(uint8 Count);
	void StartRuneSpawn();
	void StopRuneSpawn();
	
	
private:
	FVector GetRandomSpawnLocation() const;
	UGRObjectPoolSubsystem* GetObjectPoolSubsystem() const;
	UGRDataTableSubsystem* GetDataTableSubsystem() const;

	
// Member Variable	
private:
	UPROPERTY(VisibleAnywhere, Category = "변수|룬")
	TObjectPtr<URuneInfo> RuneData;

	UPROPERTY()
	TObjectPtr<AGRPlayer> Player;
	
	
private:	
	TArray<ERuneType> AllowedRuneTypes;
	FTimerHandle RuneSpawnTimerHandle;
	float RuneSpawnInterval = 3.0f;
	int32 SpawnedRuneCount = 0;
};
