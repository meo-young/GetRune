#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "GRPlayerCameraManager.generated.h"

 
UCLASS()
class GETRUNE_API AGRPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	void PlayCameraShake(float Scale, float Duration);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	TSubclassOf<UCameraShakeBase> CameraShake;

private:
	UPROPERTY()
	TObjectPtr<UCameraShakeBase> ActiveShakeInstance;

	FTimerHandle ShakeTimerHandle;

	void StopActiveShake();
	
};
