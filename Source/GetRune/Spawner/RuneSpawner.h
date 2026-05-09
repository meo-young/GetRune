#pragma once

#include "CoreMinimal.h"
#include "GRSpawnerBase.h"
#include "UObject/Object.h"
#include "GetRune/Data/CharacterInfo.h"
#include "RuneSpawner.generated.h"

class UGRDataTableSubsystem;
class UGRObjectPoolSubsystem;
class URuneInfo;
class AGRRuneBase;

UCLASS()
class GETRUNE_API URuneSpawner : public UGRSpawnerBase
{
	GENERATED_BODY()

// Lifecycle	
public:
	URuneSpawner();
	

// Member Function	
public:
	virtual void Initialize() override;
	virtual void Spawn() override;
	
	
private:
	virtual FVector GetRandomSpawnLocation() const override;

	
// Member Variable	
private:
	UPROPERTY(VisibleAnywhere, Category = "변수|룬")
	TObjectPtr<URuneInfo> RuneData;
	
	
private:	
	TArray<ERuneType> AllowedRuneTypes;
	
};
