#include "GRRuneBase.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"

void AGRRuneBase::OnPlayerOverlapped()
{
	UGRObjectPoolSubsystem* Pool = GetGameInstance()->GetSubsystem<UGRObjectPoolSubsystem>();
	Pool->ReleaseActor(this);
}

void AGRRuneBase::OnAcquiredFromPool()
{
	// GRObjectPoolSubsystem::Activate가 Tick을 켜지만,
	// 룬은 자석(OnMagnetOverlapped) 전까지 Tick이 필요 없음
	SetActorTickEnabled(false);
}

void AGRRuneBase::OnReleasedToPool()
{
}
