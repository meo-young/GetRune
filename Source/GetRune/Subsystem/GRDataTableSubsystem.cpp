#include "GRDataTableSubsystem.h"
#include "GetRune/GetRune.h"
#include "GetRune/Player/GRPlayer.h"

const FCharacterInfo* UGRDataTableSubsystem::GetCharacterInfo(TSubclassOf<AGRPlayer> PlayerClass) const
{
	UDataTable* Table = CharacterInfoTable.Get();
	if (!Table) return nullptr;

	for (const FName& RowName : Table->GetRowNames())
	{
		const FCharacterInfo* Row = Table->FindRow<FCharacterInfo>(RowName, TEXT(""));
		if (Row && Row->PlayerClass == PlayerClass)
		{
			return Row;
		}
	}
	return nullptr;
}
