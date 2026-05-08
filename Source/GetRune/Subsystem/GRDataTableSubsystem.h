#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GetRune/Data/CharacterInfo.h"
#include "GRDataTableSubsystem.generated.h"

class AGRPlayer;

UCLASS(Config = Game)
class GETRUNE_API UGRDataTableSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	const FCharacterInfo* GetCharacterInfo(TSubclassOf<AGRPlayer> PlayerClass) const;

private:
	UPROPERTY(Config)
	TSoftObjectPtr<UDataTable> CharacterInfoTable;
};
