#pragma once

#include "CoreMinimal.h"
#include "GetRune/Item/GRItemBase.h"
#include "GetRune/Data/CharacterInfo.h"
#include "GetRune/Interface/Poolable.h"
#include "GRRuneBase.generated.h"

UCLASS()
class GETRUNE_API AGRRuneBase : public AGRItemBase, public IPoolable
{
	GENERATED_BODY()

public:
	virtual void OnPlayerOverlapped() override;
	virtual void OnAcquiredFromPool() override;
	virtual void OnReleasedToPool() override;

	ERuneType RuneType = ERuneType::Red;
};
