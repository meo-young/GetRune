#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GetRune/Data/CharacterInfo.h"
#include "GRRuneSpawnComponent.generated.h"

class URuneInfo;
class AGRRuneBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GETRUNE_API UGRRuneSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

	
// Lifecycle	
public:
	UGRRuneSpawnComponent();
	virtual void BeginPlay() override;

	
// Member Function	
public:	
	void SpawnRunes(uint8 Count);

private:
	void SpawnRune();
	void StartRuneSpawn();
	void StopRuneSpawn();
	FVector GetRandomSpawnLocation() const;

	
// Member Variable	
private:
	UPROPERTY(VisibleAnywhere, Category = "변수|룬")
	TObjectPtr<URuneInfo> RuneData;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|룬")
	float InnerRadius = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = "변수|룬")
	float OuterRadius = 700.f;

private:	
	FTimerHandle RuneSpawnTimerHandle;
	float RuneSpawnInterval = 3.0f;
	TArray<ERuneType> AllowedRuneTypes;
};
