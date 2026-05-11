#include "GRHUD.h"
#include "GetRune/UI/GREnemyCounterWidget.h"
#include "GetRune/UI/GRWaveInfoWidget.h"
#include "GetRune/UI/GRRuneCounterWidget.h"
#include "GetRune/UI/GRPlayerStatusWidget.h"
#include "GetRune/GameState/GRGameState.h"
#include "GetRune/Spawner/EnemySpawner.h"
#include "GetRune/Player/GRPlayer.h"
#include "GetRune/Component/HealthComponent.h"
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
	if (RuneCounterWidgetClass)
	{
		RuneCounterWidgetInstance = CreateWidget<UGRRuneCounterWidget>(GetOwningPlayerController(), RuneCounterWidgetClass);
	}
	if (PlayerStatusWidgetClass)
	{
		PlayerStatusWidgetInstance = CreateWidget<UGRPlayerStatusWidget>(GetOwningPlayerController(), PlayerStatusWidgetClass);
	}
}

void AGRHUD::BeginPlay()
{
	Super::BeginPlay();

	AGRGameState* GRGameState = GetWorld()->GetGameState<AGRGameState>();
	UEnemySpawner* EnemySpawner = GRGameState->EnemySpawnManager;
	AGRPlayer* Player = Cast<AGRPlayer>(GetOwningPawn());

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
	if (RuneCounterWidgetInstance && Player)
	{
		Player->OnRuneCountChanged.AddDynamic(RuneCounterWidgetInstance, &UGRRuneCounterWidget::SetRuneCounter);
		RuneCounterWidgetInstance->AddToViewport();
	}
	if (PlayerStatusWidgetInstance && Player)
	{
		UHealthComponent::FindHealthComponent(Player)->OnHealthChanged.AddDynamic(PlayerStatusWidgetInstance, &UGRPlayerStatusWidget::SetHealthText);
		PlayerStatusWidgetInstance->AddToViewport();
	}
}
