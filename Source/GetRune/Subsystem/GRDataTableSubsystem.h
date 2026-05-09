#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GetRune/Data/CharacterInfo.h"
#include "GetRune/Data/StageInfo.h"
#include "GRDataTableSubsystem.generated.h"

class AGRPlayer;

UCLASS(Config = Game)
class GETRUNE_API UGRDataTableSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	const FCharacterInfo* GetCharacterInfo(TSubclassOf<AGRPlayer> PlayerClass) const;
	const FStageInfo* GetStageInfo(const int32 InStageNum) const;

private:
	UPROPERTY(Config)
	TSoftObjectPtr<UDataTable> CharacterInfoTable;
	
	UPROPERTY(Config)
	TSoftObjectPtr<UDataTable> StageInfoTable;
	
};
