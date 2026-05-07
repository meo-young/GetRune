#pragma once

#include "CoreMinimal.h"
#include "GRInputConfig.h"
#include "EnhancedInputComponent.h"
#include "GRInputComponent.generated.h"

struct FGameplayTag;
class ULOPInputConfig;

/**
 * Gameplay Tag 기반 입력 바인딩을 제공하는 InputComponent 확장 클래스.
 * - BindNativeAction : 특정 Tag에 매핑된 UInputAction을 단일 함수에 바인딩합니다.
 * - BindAbilityAction : InputConfig의 모든 어빌리티 입력을 Pressed/Released 쌍으로 일괄 바인딩합니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GETRUNE_API UGRInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
// LifeCycle Function	
public:
	UGRInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


// Member Function	
public:	
	// InputTag에 해당하는 Native InputAction을 TriggerEvent에 바인딩합니다.
	template <class UserClass, typename FuncType>
	void BindNativeAction(const UGRInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	// AbilityInputActions 전체를 Triggered(Pressed)/Completed(Released)로 일괄 바인딩하고 핸들을 BindHandles에 저장합니다.
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityAction(const UGRInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);

};

template <class UserClass, typename FuncType>
void UGRInputComponent::BindNativeAction(const UGRInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UGRInputComponent::BindAbilityAction(const UGRInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);
	
	for (const FGRInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle();
			}
			
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle();
			}
		}
	}
}
