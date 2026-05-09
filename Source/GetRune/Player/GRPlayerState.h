#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GRPlayerState.generated.h"

class UGRAbilitySystemComponent;
class UGRAbilitySet;

UCLASS()
class GETRUNE_API AGRPlayerState : public APlayerState
{
	GENERATED_BODY()
	
// LifeCycle Function	
public:
	AGRPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
	
	
// Member Function	
private:	
	void InitAbilities();
	
	
// Member Variable
private:
	UPROPERTY(EditDefaultsOnly, Category = "변수|어빌리티")
	TArray<TObjectPtr<UGRAbilitySet>> DefaultAbilitySets;
	
	UPROPERTY(VisibleAnywhere, Category = "변수|어빌리티")
	TObjectPtr<UGRAbilitySystemComponent> ASC;
	
private:
	int32 CurrentStageNum = 0;
	

// Getter, Setter	
public:
	FORCEINLINE UGRAbilitySystemComponent* GetAbilitySystemComponent() const { return ASC; }
	FORCEINLINE int32 GetCurrentStageNum() const { return CurrentStageNum; }
	
};
