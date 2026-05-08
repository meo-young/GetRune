#include "GRObjectPoolSubsystem.h"
#include "GetRune/GetRune.h"
#include "GetRune/Interface/Poolable.h"
#include "GetRune/Item/GRItemBase.h"

void UGRObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGRObjectPoolSubsystem::InitializePools(TSubclassOf<AActor> ActorClass, int32 InitialSize)
{
	UWorld* World = GetWorld();
	if (!World) return;

	// 해당 클래스의 풀을 찾거나 새로 생성합니다.
	TArray<TObjectPtr<AActor>>& Pool = PoolMap.FindOrAdd(ActorClass);

	// 지정한 수만큼 액터를 미리 스폰해 풀에 채워둡니다.
	for (int32 i = 0; i < InitialSize; ++i)
	{
		if (AActor* Actor = SpawnPooledActor(ActorClass))
		{
			Pool.Emplace(Actor);
		}
	}
}

AActor* UGRObjectPoolSubsystem::AcquireActor(TSubclassOf<AActor> ActorClass, const FVector& Location, const FRotator& Rotation)
{
	// 해당 클래스의 풀을 조회합니다.
	TArray<TObjectPtr<AActor>>* Pool = PoolMap.Find(ActorClass);
	AActor* Actor = nullptr;

	if (Pool)
	{
		// 유효한 액터를 찾을 때까지 풀에서 꺼냅니다. GC로 무효화된 항목은 건너뜁니다.
		while (Pool->Num() > 0)
		{
			TObjectPtr<AActor> Pooled = Pool->Pop(EAllowShrinking::No);
			if (IsValid(Pooled))
			{
				Actor = Pooled;
				break;
			}
		}
	}

	// 풀이 비어있으면 새로 스폰합니다.
	if (!Actor)
	{
		Actor = SpawnPooledActor(ActorClass);
	}

	// 액터를 지정 위치에 배치하고 활성화합니다.
	if (Actor)
	{
		Activate(Actor, Location, Rotation);
	}

	return Actor;
}

void UGRObjectPoolSubsystem::ReleaseActor(AActor* Actor)
{
	if (!IsValid(Actor)) return;

	// 액터를 비활성화한 뒤 해당 클래스의 풀에 반납합니다.
	Deactivate(Actor);
	PoolMap.FindOrAdd(Actor->GetClass()).Emplace(Actor);
}

AActor* UGRObjectPoolSubsystem::SpawnPooledActor(TSubclassOf<AActor> ActorClass)
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	// 충돌 여부와 관계없이 원점에 스폰합니다.
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 스폰 후 즉시 비활성화해 풀 대기 상태로 만듭니다.
	AActor* Actor = World->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
	if (Actor)
	{
		Deactivate(Actor);
	}
	return Actor;
}

void UGRObjectPoolSubsystem::Activate(AActor* Actor, const FVector& Location, const FRotator& Rotation)
{
	// 지정 위치로 이동시키고 가시성과 충돌을 켭니다.
	Actor->SetActorLocationAndRotation(Location, Rotation);
	Actor->SetActorHiddenInGame(false);
	Actor->SetActorEnableCollision(true);

	// IPoolable을 구현한 액터라면 활성화 콜백을 호출합니다.
	if (IPoolable* Poolable = Cast<IPoolable>(Actor))
	{
		Poolable->OnActivated();
	}
}

void UGRObjectPoolSubsystem::Deactivate(AActor* Actor)
{
	// IPoolable을 구현한 액터라면 비활성화 콜백을 먼저 호출합니다.
	if (IPoolable* Poolable = Cast<IPoolable>(Actor))
	{
		Poolable->OnDeactivated();
	}

	// 화면에서 숨기고 충돌을 끕니다.
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
}
