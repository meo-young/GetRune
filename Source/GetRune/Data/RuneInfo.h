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
	UPROPERTY(EditAnywhere, Category = "종류")
	TMap<ERuneType, TSubclassOf<AGRRuneBase>> RuneClass;
	
	UPROPERTY(EditAnywhere, Category = "수치")
	float InnerRadius = 400.f;
	
	UPROPERTY(EditAnywhere, Category = "수치")
	float OuterRadius = 700.f;
	
	UPROPERTY(EditAnywhere, Category = "수치")
	int32 MaxRuneCount = 100;
	
};
