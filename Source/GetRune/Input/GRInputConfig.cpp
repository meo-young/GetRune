#include "GRInputConfig.h"

UGRInputConfig::UGRInputConfig(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const UInputAction* UGRInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FGRInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	
	return nullptr;
}

const UInputAction* UGRInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FGRInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	
	return nullptr;
}