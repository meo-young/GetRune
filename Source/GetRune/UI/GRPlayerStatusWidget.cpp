#include "GRPlayerStatusWidget.h"
#include "Components/TextBlock.h"
#include "GetRune/Component/HealthComponent.h"

void UGRPlayerStatusWidget::SetHealthText(UHealthComponent* HC, float OldValue, float NewValue, AActor* Instigator)
{
	CurrentPlayerHealth->SetText(FText::Format(FText::FromString(TEXT("HP : {0}")), FText::AsNumber(FMath::FloorToInt(NewValue))));
}
