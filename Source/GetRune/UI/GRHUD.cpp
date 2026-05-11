#include "GRHUD.h"
#include "GetRune/UI/GREnemyCounterWidget.h"
#include "GetRune/UI/GRWaveInfoWidget.h"
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
	if (WaveInfoWidgetClass)
	{
		WaveInfoWidgetInstance = CreateWidget<UGRWaveInfoWidget>(GetOwningPlayerController(), WaveInfoWidgetClass);
	}
}

void AGRHUD::BeginPlay()
{
	Super::BeginPlay();

	AGRGameState* GRGameState = GetWorld()->GetGameState<AGRGameState>();
	UEnemySpawner* EnemySpawner = GRGameState->EnemySpawnManager;

	if (EnemyCounterWidgetInstance)
	{
		EnemySpawner->OnEnemyCountChanged.AddDynamic(EnemyCounterWidgetInstance, &UGREnemyCounterWidget::SetEnemyCount);
		EnemyCounterWidgetInstance->AddToViewport();
	}
	if (WaveInfoWidgetInstance)
	{
		EnemySpawner->OnWaveChanged.AddDynamic(WaveInfoWidgetInstance, &UGRWaveInfoWidget::SetWaveNum);
		WaveInfoWidgetInstance->AddToViewport();
	}
}
