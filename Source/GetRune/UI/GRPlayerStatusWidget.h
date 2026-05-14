#pragma once

#include "CoreMinimal.h"
#include "GRUserWidget.h"
#include "GRPlayerStatusWidget.generated.h"

class UTextBlock;
class UHealthComponent;

UCLASS()
class GETRUNE_API UGRPlayerStatusWidget : public UGRUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetHealthText(UHealthComponent* HC, float OldValue, float NewValue, AActor* Instigator);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayVignetteEffect();
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentPlayerHealth;
	
};
