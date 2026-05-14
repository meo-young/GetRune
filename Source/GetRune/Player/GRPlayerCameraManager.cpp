#include "GRPlayerCameraManager.h"
#include "GetRune/GetRune.h"

void AGRPlayerCameraManager::PlayCameraShake(float Scale, float Duration)
{
	StopActiveShake();
	ActiveShakeInstance = StartCameraShake(CameraShake, Scale);
	GetWorldTimerManager().SetTimer(ShakeTimerHandle, this, &AGRPlayerCameraManager::StopActiveShake, Duration, false);
}

void AGRPlayerCameraManager::StopActiveShake()
{
	if (ActiveShakeInstance)
	{
		StopCameraShake(ActiveShakeInstance, false);
		ActiveShakeInstance = nullptr;
	}
}
