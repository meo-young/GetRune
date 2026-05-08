#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RuneInfo.generated.h"

class AGRRuneBase;
class AGRGreenRune;
class AGRBlueRune;
class AGRRedRune;

UENUM(BlueprintType)
enum class ERuneType : uint8
{
	Red,
	Green,
	Blue
};

UCLASS()
class GETRUNE_API URuneInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TMap<ERuneType, TSubclassOf<AGRRuneBase>> RuneClass;
	
};
