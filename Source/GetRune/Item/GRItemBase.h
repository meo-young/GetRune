#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GetRune/Interface/Pickupable.h"
#include "GetRune/Interface/Poolable.h"
#include "GRItemBase.generated.h"

class UBoxComponent;

UCLASS()
class GETRUNE_API AGRItemBase : public AActor, public IPickupable, public IPoolable
{
	GENERATED_BODY()
	
// Lifecycle
public:
	AGRItemBase();
	virtual void Tick(float DeltaTime) override;
	
	
// IPickupable Interface	
public:	
	virtual void OnMagnetOverlapped() override;
	virtual void OnPlayerOverlapped() override;
	
	
// IPoolable Interface	
public:
	virtual void OnActivated() override;
	virtual void OnDeactivated() override;

	
// Magnet
private:
	void MagnetAttract(const float InDeltaTime);

	
// Component
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Collision;

	
private:
	float CurrentMagnetSpeed = 0.f;
	uint8 bIsAttracting : 1 = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Magnet")
	float MagnetInitialSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Magnet")
	float MagnetAcceleration = 800.f;
	

};
