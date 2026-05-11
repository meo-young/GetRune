#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GRHUD.generated.h"

class UGREnemyCounterWidget;
class UGRWaveInfoWidget;
class UGRRuneCounterWidget;
class UGRPlayerStatusWidget;

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

	UPROPERTY()
	TObjectPtr<UGRWaveInfoWidget> WaveInfoWidgetInstance;

	UPROPERTY()
	TObjectPtr<UGRRuneCounterWidget> RuneCounterWidgetInstance;

	UPROPERTY()
	TObjectPtr<UGRPlayerStatusWidget> PlayerStatusWidgetInstance;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UGREnemyCounterWidget> EnemyCounterWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UGRWaveInfoWidget> WaveInfoWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UGRRuneCounterWidget> RuneCounterWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UGRPlayerStatusWidget> PlayerStatusWidgetClass;
	
};
