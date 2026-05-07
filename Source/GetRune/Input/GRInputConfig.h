#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GRInputConfig.generated.h"

class UInputAction;
struct FGameplayTag;

// GameplayTag와 InputAction을 연결하는 래퍼 클래스입니다.
USTRUCT(BlueprintType)
struct FGRInputAction
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

UCLASS()
class GETRUNE_API UGRInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
// LifeCycle Function	
public:
	UGRInputConfig(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	
// Member Function	
public:	
	// 잘 바뀌지 않는 InputAction을 NativeInputAction, 자주 바뀌는 InputAction을 AbilityInputAction으로 구분하여 관리합니다.
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;
	
	
// Member Variable
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction", AllowPrivateAccess = true))
	TArray<FGRInputAction> NativeInputActions;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction", AllowPrivateAccess = true))
	TArray<FGRInputAction> AbilityInputActions;
	
};
