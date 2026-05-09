#include "GRAttributeSet.h"
#include "GetRune/GetRune.h"
#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"

UGRAttributeSet::UGRAttributeSet()
{
}

UWorld* UGRAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);
	
	LOG(TEXT("Outer Name : %s"), *Outer->GetName());

	return Outer->GetWorld();
}

UGRAbilitySystemComponent* UGRAttributeSet::GetGRAbilitySystemComponent() const
{
	return Cast<UGRAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
