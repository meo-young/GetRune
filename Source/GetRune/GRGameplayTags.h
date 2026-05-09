#pragma once

#include "NativeGameplayTags.h"

namespace GRGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	
	extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Heal);
}
