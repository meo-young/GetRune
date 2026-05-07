#include "GRPlayerState.h"
#include "GetRune/AbilitySystem/GRAbilitySet.h"
#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"

AGRPlayerState::AGRPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGRAbilitySystemComponent>(this, "AbilitySystemComponent");
}

void AGRPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilities();
}

void AGRPlayerState::InitAbilities()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
	
	for (UGRAbilitySet* AbilitySet : DefaultAbilitySets)
	{
		if (AbilitySet)
		{
			// OutGrantedHandle을 nullptr로 지정한다는 것은 영구 부여한다는 의미입니다.
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}