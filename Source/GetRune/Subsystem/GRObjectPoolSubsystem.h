#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GRObjectPoolSubsystem.generated.h"

USTRUCT()
struct FActorPool
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<AActor>> Actors;
};

UCLASS()
class GETRUNE_API UGRObjectPoolSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	void InitializePools(TSubclassOf<AActor> ActorClass, int32 InitialSize);

	AActor* AcquireActor(TSubclassOf<AActor> ActorClass, const FVector& Location, const FRotator& Rotation);

	void ReleaseActor(AActor* Actor);

private:
	AActor* SpawnPooledActor(TSubclassOf<AActor> ActorClass);
	void Activate(AActor* Actor, const FVector& Location, const FRotator& Rotation);
	void Deactivate(AActor* Actor);

	void HandleWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources);

private:
	UPROPERTY()
	TMap<TSubclassOf<AActor>, FActorPool> PoolMap;

	FDelegateHandle WorldCleanupHandle;
};
