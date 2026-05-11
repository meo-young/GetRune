#pragma once

#include "CoreMinimal.h"
#include "GRUserWidget.h"
#include "GREnemyStatusWidget.generated.h"

class UProgressBar;
class UHealthComponent;

UCLASS()
class GETRUNE_API UGREnemyStatusWidget : public UGRUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthRatio(const float CurrentHealthRatio);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> EnemyHealthBar;
};
