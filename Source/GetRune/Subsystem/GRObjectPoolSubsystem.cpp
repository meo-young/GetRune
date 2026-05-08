#include "GRObjectPoolSubsystem.h"
#include "GetRune/Interface/Poolable.h"

void UGRObjectPoolSubsystem::InitializePools(const TArray<FGRPoolConfig>& Configs)
{
	UWorld* World = GetWorld();
	if (!World) return;

	for (const FGRPoolConfig& Config : Configs)
	{
		if (!Config.ActorClass) continue;

		TArray<TObjectPtr<AActor>>& Pool = PoolMap.FindOrAdd(Config.ActorClass);
		Pool.Reserve(Config.InitialSize);

		for (int32 i = 0; i < Config.InitialSize; ++i)
		{
			if (AActor* Actor = SpawnPooledActor(Config.ActorClass))
			{
				Pool.Add(Actor);
			}
		}
	}
}

AActor* UGRObjectPoolSubsystem::AcquireActor(TSubclassOf<AActor> ActorClass, const FVector& Location, const FRotator& Rotation)
{
	TArray<TObjectPtr<AActor>>* Pool = PoolMap.Find(ActorClass);
	AActor* Actor = nullptr;

	if (Pool)
	{
		for (TObjectPtr<AActor>& Pooled : *Pool)
		{
			if (Pooled && Pooled->IsHidden())
			{
				Actor = Pooled;
				break;
			}
		}
	}

	if (!Actor)
	{
		Actor = SpawnPooledActor(ActorClass);
		if (Actor)
		{
			PoolMap.FindOrAdd(ActorClass).Add(Actor);
		}
	}

	if (Actor)
	{
		Activate(Actor, Location, Rotation);
	}

	return Actor;
}

void UGRObjectPoolSubsystem::ReleaseActor(AActor* Actor)
{
	if (Actor)
	{
		Deactivate(Actor);
	}
}

AActor* UGRObjectPoolSubsystem::SpawnPooledActor(TSubclassOf<AActor> ActorClass)
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Actor = World->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
	if (Actor)
	{
		Deactivate(Actor);
	}
	return Actor;
}

void UGRObjectPoolSubsystem::Activate(AActor* Actor, const FVector& Location, const FRotator& Rotation)
{
	Actor->SetActorLocationAndRotation(Location, Rotation);
	Actor->SetActorHiddenInGame(false);
	Actor->SetActorEnableCollision(true);

	if (IPoolable* Poolable = Cast<IPoolable>(Actor))
	{
		Poolable->OnAcquiredFromPool();
	}
}

void UGRObjectPoolSubsystem::Deactivate(AActor* Actor)
{
	if (IPoolable* Poolable = Cast<IPoolable>(Actor))
	{
		Poolable->OnReleasedToPool();
	}

	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
}
