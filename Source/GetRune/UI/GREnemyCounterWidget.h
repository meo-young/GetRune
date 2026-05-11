#pragma once

#include "CoreMinimal.h"
#include "GRUserWidget.h"
#include "GREnemyCounterWidget.generated.h"

class UTextBlock;

UCLASS()
class GETRUNE_API UGREnemyCounterWidget : public UGRUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetEnemyCount(const int32 CurrentEnemyCount);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EnemyCount;
	
};
