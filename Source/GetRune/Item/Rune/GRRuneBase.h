#pragma once

#include "CoreMinimal.h"
#include "GetRune/Item/GRItemBase.h"
#include "GetRune/Data/CharacterInfo.h"
#include "GetRune/Interface/Poolable.h"
#include "GRRuneBase.generated.h"

UCLASS()
class GETRUNE_API AGRRuneBase : public AGRItemBase
{
	GENERATED_BODY()

public:
	ERuneType RuneType = ERuneType::Red;
	
};
