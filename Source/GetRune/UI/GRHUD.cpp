#include "GRHUD.h"
#include "GetRune/UI/GREnemyCounterWidget.h"
#include "GetRune/GameState/GRGameState.h"
#include "GetRune/Spawner/EnemySpawner.h"
#include "Blueprint/UserWidget.h"

void AGRHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (EnemyCounterWidgetClass)
	{
		EnemyCounterWidgetInstance = CreateWidget<UGREnemyCounterWidget>(GetOwningPlayerController(), EnemyCounterWidgetClass);
	}
}

void AGRHUD::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyCounterWidgetInstance)
	{
		AGRGameState* GRGameState = GetWorld()->GetGameState<AGRGameState>();
		GRGameState->EnemySpawnManager->OnEnemyCountChanged.AddDynamic(EnemyCounterWidgetInstance, &UGREnemyCounterWidget::SetEnemyCount);
		EnemyCounterWidgetInstance->AddToViewport();
	}
}
