#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GRHUD.generated.h"

class UGREnemyCounterWidget;

UCLASS()
class GETRUNE_API AGRHUD : public AHUD
{
	GENERATED_BODY()

// Lifecycle	
public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
	
// Member Variable	
public:
	UPROPERTY()
	TObjectPtr<UGREnemyCounterWidget> EnemyCounterWidgetInstance;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UGREnemyCounterWidget> EnemyCounterWidgetClass;
	
};
