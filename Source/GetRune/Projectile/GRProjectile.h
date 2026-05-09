#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GetRune/Interface/Poolable.h"
#include "GRProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UGameplayEffect;
class UAbilitySystemComponent;
class UNiagaraComponent;

UCLASS()
class GETRUNE_API AGRProjectile : public AActor, public IPoolable
{
	GENERATED_BODY()

// Lifecycle
public:
	AGRProjectile();


// IPoolable
public:
	virtual void OnActivated() override;
	virtual void OnDeactivated() override;


// Member Function
public:
	void Launch(const FVector& Direction, float InDamage, float InSpeed,
	            TSubclassOf<UGameplayEffect> InDamageGE, UAbilitySystemComponent* InSourceASC);

private:
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                     bool bFromSweep, const FHitResult& SweepResult);

	void ReturnToPool();


// Component
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collision;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> ProjectileEffect;


// Member Variable
private:
	TSubclassOf<UGameplayEffect> DamageGE;
	float Damage = 0.f;
	TWeakObjectPtr<UAbilitySystemComponent> SourceASC;

	FTimerHandle LifetimeHandle;

	UPROPERTY(EditDefaultsOnly, Category = "변수")
	float Lifetime = 5.f;

};
