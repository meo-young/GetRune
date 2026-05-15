#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GRAnimInstance.generated.h"

class UGRAbilitySystemComponent;
class UCharacterMovementComponent;
class AGRCharacter;

UCLASS()
class GETRUNE_API UGRAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
// Lifecycle
public:
	UGRAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
	
// Cached	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AGRCharacter> Character;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	

// State Variable
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float MoveSpeedRatio;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsDead;

	// Blueprint에 노출하지 않음 — 게임 스레드에서만 접근
	UPROPERTY()
	TObjectPtr<UGRAbilitySystemComponent> ASC;
};
