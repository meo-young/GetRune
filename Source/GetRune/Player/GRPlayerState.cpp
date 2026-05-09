#include "GRPlayerState.h"
#include "GetRune/AbilitySystem/GRAbilitySet.h"
#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"
#include "GetRune/AbilitySystem/Attributes/GRCombatSet.h"
#include "GetRune/AbilitySystem/Attributes/GRHealthSet.h"

AGRPlayerState::AGRPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ASC = ObjectInitializer.CreateDefaultSubobject<UGRAbilitySystemComponent>(this, "AbilitySystemComponent");
	CreateDefaultSubobject<UGRHealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<UGRCombatSet>(TEXT("CombatSet"));
}

void AGRPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilities();
}

void AGRPlayerState::InitAbilities()
{
	ASC->InitAbilityActorInfo(this, GetPawn());
	
	for (UGRAbilitySet* AbilitySet : DefaultAbilitySets)
	{
		if (AbilitySet)
		{
			// OutGrantedHandle을 nullptr로 지정한다는 것은 영구 부여한다는 의미입니다.
			AbilitySet->GiveToAbilitySystem(ASC, nullptr);
		}
	}
}