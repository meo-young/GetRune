#include "HealthComponent.h"

#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"
#include "GetRune/AbilitySystem/Attributes/GRHealthSet.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
	ASC = nullptr;
	HealthSet = nullptr;
}

void UHealthComponent::OnUnregister()
{
	UninitializeWithAbilitySystem();
	
	Super::OnUnregister();
}

void UHealthComponent::InitializeWithAbilitySystem(UGRAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);
	
	// AbilitySystemComponent 및 HealthSet을 초기화합니다.
	ASC = InASC;
	HealthSet = ASC->GetSet<UGRHealthSet>();

	// HealthSet의 델리게이트에 이벤트를 등록합니다.
	HealthSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	HealthSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);

	// 현재 체력을 최대 체력 수치로 초기화합니다.
	ASC->SetNumericAttributeBase(UGRHealthSet::GetHealthAttribute(), HealthSet->GetMaxHealth());
	
	// HealthSet외의 다른 클래스는 HealthComponent의 델리게이트를 중심으로 소통합니다.
	// 즉, HealthComponent에 이벤트를 등록하고, HealthComponent의 델리게이트를 브로드캐스팅 하는 책임은 HealthSet에게 전달합니다.
	OnHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
	OnMaxHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
}

void UHealthComponent::UninitializeWithAbilitySystem()
{
	if (HealthSet)
	{
		HealthSet->OnHealthChanged.RemoveAll(this);
		HealthSet->OnMaxHealthChanged.RemoveAll(this);
		HealthSet->OnOutOfHealth.RemoveAll(this);
	}

	HealthSet = nullptr;
	ASC = nullptr;
}

float UHealthComponent::GetHealth() const
{
	return (HealthSet ? HealthSet->GetHealth() : 0.0f);
}

float UHealthComponent::GetMaxHealth() const
{
	return (HealthSet ? HealthSet->GetMaxHealth() : 0.0f);
}

float UHealthComponent::GetHealthNormalized() const
{
	if (HealthSet)
	{
		const float Health = HealthSet->GetHealth();
		const float MaxHealth = HealthSet->GetMaxHealth();

		return ((MaxHealth > 0.0f) ? (Health / MaxHealth) : 0.0f);
	}

	return 0.0f;
}

void UHealthComponent::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UHealthComponent::HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnMaxHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnDeath.Broadcast(this, OldValue, NewValue, DamageInstigator);
}
