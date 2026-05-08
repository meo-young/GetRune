#include "GRDataTableSubsystem.h"
#include "GetRune/GetRune.h"
#include "GetRune/Player/GRPlayer.h"

const FCharacterInfo* UGRDataTableSubsystem::GetCharacterInfo(TSubclassOf<AGRPlayer> PlayerClass) const
{
	// 데이터 테이블을 동기 로드합니다. 메모리에 없으면 디스크에서 즉시 읽어옵니다.
	UDataTable* Table = CharacterInfoTable.LoadSynchronous();
	if (!Table) return nullptr;

	// 모든 행을 순회하며 PlayerClass가 일치하는 행을 반환합니다.
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
