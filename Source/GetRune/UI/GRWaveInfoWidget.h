#pragma once

#include "CoreMinimal.h"
#include "GRUserWidget.h"
#include "GRWaveInfoWidget.generated.h"

class UTextBlock;

UCLASS()
class GETRUNE_API UGRWaveInfoWidget : public UGRUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetWaveNum(const int32 WaveNum);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentWaveNum;
	
};
