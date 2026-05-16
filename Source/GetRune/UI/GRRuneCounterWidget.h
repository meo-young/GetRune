#pragma once

#include "CoreMinimal.h"
#include "GRUserWidget.h"
#include "GRRuneCounterWidget.generated.h"

enum class ERuneType : uint8;
class UTextBlock;

UCLASS()
class GETRUNE_API UGRRuneCounterWidget : public UGRUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetRuneCounter(const int32 CurrentRuneNum, const int32 MaxRuneNum, const ERuneType SkillRuneType);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetRuneOrbPercentage(const float InPercentage, ERuneType InRuneType);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentRuneCounter;
};
