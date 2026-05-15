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
	AGRRuneBase();
	virtual void BeginPlay() override;
	
public:
	virtual void OnPlayerOverlapped() override;
	
	
// Component
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> RuneMesh;	
	

public:
	ERuneType RuneType = ERuneType::Red;
	
};
