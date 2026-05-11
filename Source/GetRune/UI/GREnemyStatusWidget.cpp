#include "GREnemyStatusWidget.h"
#include "Components/ProgressBar.h"

void UGREnemyStatusWidget::SetHealthRatio(const float CurrentHealthRatio)
{
	EnemyHealthBar->SetPercent(CurrentHealthRatio);
}
