#pragma once

#include "CoreMinimal.h"
#include "GetRune/Item/GRItemBase.h"
#include "GetRune/Data/CharacterInfo.h"
#include "GetRune/Interface/Poolable.h"
#include "GRRuneBase.generated.h"

class URuneSpawner;

UCLASS()
class GETRUNE_API AGRRuneBase : public AGRItemBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
public:
	virtual void OnPlayerOverlapped() override;

public:
	ERuneType RuneType = ERuneType::Red;
	
private:
	UPROPERTY()
	TObjectPtr<URuneSpawner> RuneSpawner;
	
};
