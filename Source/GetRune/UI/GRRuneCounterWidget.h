#pragma once

#include "CoreMinimal.h"
#include "GRUserWidget.h"
#include "GRRuneCounterWidget.generated.h"

class UTextBlock;

UCLASS()
class GETRUNE_API UGRRuneCounterWidget : public UGRUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetRuneCounter(const int32 CurrentRuneNum, const int32 MaxRuneNum);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentRuneCounter;
};
