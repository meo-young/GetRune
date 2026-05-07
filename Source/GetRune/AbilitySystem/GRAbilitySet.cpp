#include "GRAbilitySet.h"
#include "GameplayAbilitySpecHandle.h"
#include "GRAbilitySystemComponent.h"

void FAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FAbilitySet_GrantedHandles::TakeFromAbilitySystem(UGRAbilitySystemComponent* ASC)
{
	if (!ASC->IsOwnerActorAuthoritative())
	{
		return;
	}
	
	// 부여한 모든 AbilityHandle을 순회하면서 제거한다.
	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			ASC->ClearAbility(Handle);
		}
	}
}

UGRAbilitySet::UGRAbilitySet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGRAbilitySet::GiveToAbilitySystem(UGRAbilitySystemComponent* ASC, FAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	check(ASC);
	
	if (!ASC->IsOwnerActorAuthoritative())
	{
		return;
	}
	
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		if (!IsValid(AbilityToGrant.Ability))
		{
			continue;
		}
		
		// CDO는 값이 정해져 있기 때문에 플레이어마다 다르게 게임 어빌리티를 적용하는 것이 불가능하다. 하지만 가볍다.
		UGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbility>();
		
		// 어빌리티를 Spec으로 묶어준다. Ability를 어떤 태그와 매칭할지를 결정한다.
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);
		
		// 핸들을 반환 받아서 추후 제거 작업에 사용한다.
		// 인스턴스끼리 비교하는 거에 비해 훨씬 빠르다.
		const FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}
}