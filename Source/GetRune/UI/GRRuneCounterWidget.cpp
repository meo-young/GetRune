#include "GRRuneCounterWidget.h"
#include "Components/TextBlock.h"
#include "GetRune/Data/RuneInfo.h"

void UGRRuneCounterWidget::SetRuneCounter(const int32 CurrentRuneNum, const int32 MaxRuneNum, const ERuneType SkillRuneType)
{
	CurrentRuneCounter->SetText(FText::Format(
		FText::FromString(TEXT("{0} / {1}")),
		FText::AsNumber(CurrentRuneNum),
		FText::AsNumber(MaxRuneNum)));

	const float Percentage = MaxRuneNum > 0 ? static_cast<float>(CurrentRuneNum) / MaxRuneNum : 0.f;
	SetRuneOrbPercentage(Percentage, SkillRuneType);
}
