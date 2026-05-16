#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GRGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8
{
	// Input이 Trigger 됐을 경우 (Pressed/Released)
	OnInputTriggered,
	
	// Input이 Held되어 있을 경우
	WhileInputActive,
	
	// Avatar가 생성됐을 경우 바로 할당 (패시브 스킬)
	OnSpawn
};

UCLASS()
class GETRUNE_API UGRGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
// LifeCycle Function	
public:
	UGRGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	
// Member Function	
protected:
	// 카메라 모드를 설정합니다.
	// UFUNCTION(BlueprintCallable)
	// void SetCameraMode(TSubclassOf<UGRCameraMode> CameraMode);
	//
	// 어빌리티로 인해 활성화 되어 있는 카메라 모드를 초기화합니다. EndAbility 에서 자동으로 호출됩니다.
	// UFUNCTION(BlueprintCallable)
	// void ClearCameraMode();


// Member Variable	
public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|정책", meta = (AllowPrivateAccess = true))
	EAbilityActivationPolicy ActivationPolicy;
	
private:
	//TSubclassOf<ULOPCameraMode> ActiveCameraMode;
	
};
