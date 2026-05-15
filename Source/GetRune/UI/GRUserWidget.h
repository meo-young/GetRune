#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GRUserWidget.generated.h"

UCLASS()
class GETRUNE_API UGRUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
};
