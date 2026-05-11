#include "GRWaveInfoWidget.h"
#include "Components/TextBlock.h"

void UGRWaveInfoWidget::SetWaveNum(const int32 WaveNum)
{
	CurrentWaveNum->SetText(FText::Format(FText::FromString(TEXT("Wave {0}")), FText::AsNumber(WaveNum)));
}
