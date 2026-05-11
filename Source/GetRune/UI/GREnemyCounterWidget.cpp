#include "GREnemyCounterWidget.h"
#include "Components/TextBlock.h"

void UGREnemyCounterWidget::SetEnemyCount(const int32 CurrentEnemyCount)
{
	EnemyCount->SetText(FText::AsNumber(CurrentEnemyCount));
}
