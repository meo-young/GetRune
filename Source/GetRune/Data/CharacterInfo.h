#pragma once

#include "CoreMinimal.h"
#include "RuneInfo.h"
#include "CharacterInfo.generated.h"

class AGRPlayer;

USTRUCT()
struct GETRUNE_API FCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()

	/** 직업 클래스입니다. */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGRPlayer> PlayerClass;
	
	/** 캐릭터의 이동속도입니다. */
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 600.0f;
	
	/** 에너지를 끌어당기기 시작하는 범위입니다. */
	UPROPERTY(EditAnywhere)
	float MagnetRadius = 200.0f;

	/** 스킬 발동을 위해 필요한 에너지 개수입니다. */
	UPROPERTY(EditAnywhere)
	uint8 RequiredRuneCount = 3;
	
	/** 캐릭터 주변으로 에너지를 생성하는 간격입니다. */
	UPROPERTY(EditAnywhere)
	float RuneSpawnInterval = 3.0f;

	/** 빨간색 에너지를 생성할지에 대한 여부입니다. */
	UPROPERTY(EditAnywhere, DisplayName = "Red")
	bool bRed = true;

	/** 초록색 에너지를 생성할지에 대한 여부입니다. */
	UPROPERTY(EditAnywhere, DisplayName = "Green")
	bool bGreen = true;

	/** 파란색 에너지를 생성할지에 대한 여부입니다. */
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
