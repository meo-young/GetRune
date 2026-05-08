#pragma once

#include "CoreMinimal.h"
#include "RuneInfo.h"
#include "CharacterInfo.generated.h"

class AGRPlayer;

USTRUCT()
struct GETRUNE_API FCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGRPlayer> PlayerClass;
	
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 600.0f;

	UPROPERTY(EditAnywhere)
	uint8 RequiredRuneCount = 3;
	
	UPROPERTY(EditAnywhere)
	float RuneSpawnInterval = 3.0f;

	UPROPERTY(EditAnywhere, DisplayName = "Red")
	bool bRed = true;

	UPROPERTY(EditAnywhere, DisplayName = "Green")
	bool bGreen = true;

	UPROPERTY(EditAnywhere, DisplayName = "Blue")
	bool bBlue = true;

	TArray<ERuneType> GetAllowedRuneTypes() const
	{
		TArray<ERuneType> Result;
		if (bRed)   Result.Add(ERuneType::Red);
		if (bGreen) Result.Add(ERuneType::Green);
		if (bBlue)  Result.Add(ERuneType::Blue);
		return Result;
	}
};
