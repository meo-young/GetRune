#include "GRRuneCounterWidget.h"
#include "Components/TextBlock.h"

void UGRRuneCounterWidget::SetRuneCounter(const int32 CurrentRuneNum, const int32 MaxRuneNum)
{
	CurrentRuneCounter->SetText(FText::Format(
		FText::FromString(TEXT("{0} / {1}")),
		FText::AsNumber(CurrentRuneNum),
		FText::AsNumber(MaxRuneNum)));
}
