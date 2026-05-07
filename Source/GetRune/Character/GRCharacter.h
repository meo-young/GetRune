#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GRCharacter.generated.h"

UCLASS()
class GETRUNE_API AGRCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
// Lifecycle Section
public:
	AGRCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	
	
// IAbilitySystemInterface	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	
// Member Function	
protected:
	void SetMovementModeTag(EMovementMode CurrentMovementMode, uint8 CurrentCustomMode, bool bTagEnabled);
	
};
