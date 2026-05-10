#include "GRAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"
#include "GetRune/Character/GRCharacter.h"

UGRAnimInstance::UGRAnimInstance()
{
}

void UGRAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Character = Cast<AGRCharacter>(GetOwningActor());
	
	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UGRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!Character || !MovementComponent) return;
	
	ASC = Cast<UGRAbilitySystemComponent>(Character->GetAbilitySystemComponent());
	
	const float Velocity = Character->GetVelocity().Length();
	
	MoveSpeedRatio = Velocity/MovementComponent->GetMaxSpeed();
}
