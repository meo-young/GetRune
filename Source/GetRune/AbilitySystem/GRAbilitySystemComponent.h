#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GRAbilitySystemComponent.generated.h"

UCLASS()
class GETRUNE_API UGRAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
// LifeCycle Function	
public:
	UGRAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	

// Member Function	
public:	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	
	
// Member Variable	
private:	
	// Ability Input 처리할 Pending Queue
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
};
