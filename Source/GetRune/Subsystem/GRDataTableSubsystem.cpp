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

const FStageInfo* UGRDataTableSubsystem::GetStageInfo(const int32 InStageNum) const
{
	// 데이터 테이블을 동기 로드합니다. 메모리에 없으면 디스크에서 즉시 읽어옵니다.
	UDataTable* Table = StageInfoTable.LoadSynchronous();
	if (!Table) return nullptr;

	// 행 이름 목록을 가져와 InStageNum을 인덱스로 사용합니다.
	const TArray<FName> RowNames = Table->GetRowNames();
	if (!RowNames.IsValidIndex(InStageNum)) return nullptr;

	return Table->FindRow<FStageInfo>(RowNames[InStageNum], TEXT(""));
}
