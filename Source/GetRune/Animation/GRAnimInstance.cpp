#include "GRAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GetRune/GRGameplayTags.h"
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
		ASC = Cast<UGRAbilitySystemComponent>(Character->GetAbilitySystemComponent());
	}
}

void UGRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character || !MovementComponent) return;

	if (!ASC)
	{
		ASC = Cast<UGRAbilitySystemComponent>(Character->GetAbilitySystemComponent());
		return;
	}

	// Blueprint용 상태를 게임 스레드에서 미리 계산
	bIsDead = ASC->HasMatchingGameplayTag(GRGameplayTags::Status_Death);
}

void UGRAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!Character || !MovementComponent) return;

	const float Velocity = Character->GetVelocity().Length();
	MoveSpeedRatio = Velocity / MovementComponent->GetMaxSpeed();
}
