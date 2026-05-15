#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GRCharacterMovementComponent.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_MovementStopped);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GETRUNE_API UGRCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
// UGRCharacterMovementComponent Interface	
public:
	virtual FRotator GetDeltaRotation(float DeltaTime) const override;
	virtual float GetMaxSpeed() const override;


};
