#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GRObjectPoolSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FGRPoolConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InitialSize = 10;
};

UCLASS()
class GETRUNE_API UGRObjectPoolSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void InitializePools(const TArray<FGRPoolConfig>& Configs);

	AActor* AcquireActor(TSubclassOf<AActor> ActorClass, const FVector& Location, const FRotator& Rotation);

	void ReleaseActor(AActor* Actor);

private:
	AActor* SpawnPooledActor(TSubclassOf<AActor> ActorClass);
	void Activate(AActor* Actor, const FVector& Location, const FRotator& Rotation);
	void Deactivate(AActor* Actor);

private:
	TMap<TSubclassOf<AActor>, TArray<TObjectPtr<AActor>>> PoolMap;
};
