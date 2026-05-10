#pragma once

#include "CoreMinimal.h"
#include "RuneInfo.h"
#include "SkillInfo.h"
#include "CharacterInfo.generated.h"

class AGRPlayer;

USTRUCT()
struct GETRUNE_API FCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()

	/** 직업 클래스입니다. */ 
	UPROPERTY(EditAnywhere, meta = (DisplayName = "직업"))
	TSubclassOf<AGRPlayer> PlayerClass;
	
	/** 캐릭터의 이동속도입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "이동 속도"))
	float MoveSpeed = 600.0f;
	
	/** 에너지를 끌어당기기 시작하는 범위입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "자석 범위"))
	float MagnetRadius = 200.0f;

	/** 스킬 발동을 위해 필요한 에너지 개수입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "스킬 발동 최대 룬 개수"))
	uint8 RequiredRuneCount = 15;
	
	/** 캐릭터 주변으로 에너지를 생성하는 간격입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "룬 생성 간격"))
	float RuneSpawnInterval = 3.0f;
	
	/** 첫 번째 룬에 대한 스킬 정보입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "첫 번째 룬"))
	FRuneSkillEntry RuneSkillData_1;

	/** 두 번째 룬에 대한 스킬 정보입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "두 번째 룬"))
	FRuneSkillEntry RuneSkillData_2;

	/** 세 번째 룬에 대한 스킬 정보입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "세 번째 룬"))
	FRuneSkillEntry RuneSkillData_3;

	TArray<ERuneType> GetAllowedRuneTypes() const
	{
		return { RuneSkillData_1.RuneType, RuneSkillData_2.RuneType, RuneSkillData_3.RuneType };
	}
};
