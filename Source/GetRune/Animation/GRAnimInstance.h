#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GRAnimInstance.generated.h"

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
};
