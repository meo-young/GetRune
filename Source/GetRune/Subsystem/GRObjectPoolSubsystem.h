#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GRObjectPoolSubsystem.generated.h"

UCLASS()
class GETRUNE_API UGRObjectPoolSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
public:
	void InitializePools(TSubclassOf<AActor> ActorClass, int32 InitialSize);

	AActor* AcquireActor(TSubclassOf<AActor> ActorClass, const FVector& Location, const FRotator& Rotation);

	void ReleaseActor(AActor* Actor);

private:
	AActor* SpawnPooledActor(TSubclassOf<AActor> ActorClass);
	void Activate(AActor* Actor, const FVector& Location, const FRotator& Rotation);
	void Deactivate(AActor* Actor);

private:
	TMap<TSubclassOf<AActor>, TArray<TObjectPtr<AActor>>> PoolMap;
};
