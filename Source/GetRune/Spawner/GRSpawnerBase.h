#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GRSpawnerBase.generated.h"

class AGRPlayer;
class UGRDataTableSubsystem;
class UGRObjectPoolSubsystem;

UCLASS(Abstract)
class GETRUNE_API UGRSpawnerBase : public UObject
{
	GENERATED_BODY()
	
	
// Member Function	
public:
	virtual void Initialize();
	virtual void Spawn() {};
	void SpawnMultiple(uint8 Count);
	virtual void StartSpawn();
	virtual void StopSpawn();
	
	
protected:
	virtual FVector GetRandomSpawnLocation() const { return FVector::ZeroVector; };
	UGRObjectPoolSubsystem* GetObjectPoolSubsystem() const;
	UGRDataTableSubsystem* GetDataTableSubsystem() const;
	
	
// Member Variable	
public:
	int32 SpawnedCount = 0;
	
protected:
	UPROPERTY()
	TObjectPtr<AGRPlayer> Player;
	
protected:
	FTimerHandle SpawnTimerHandle;
	float SpawnInterval = 3.0f;
	
};
