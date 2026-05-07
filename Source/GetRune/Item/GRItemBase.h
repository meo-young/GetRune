#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GetRune/Interface/Pickupable.h"
#include "GRItemBase.generated.h"

class UBoxComponent;

UCLASS()
class GETRUNE_API AGRItemBase : public AActor, public IPickupable
{
	GENERATED_BODY()
	
// Lifecycle
public:
	AGRItemBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	
// IPickupable Interface	
public:	
	virtual void OnMagnetOverlapped() override;
	virtual void OnPlayerOverlapped() override;

	
// Magnet
private:
	void StartMagnetAttract();

private:
	float CurrentMagnetSpeed = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Magnet")
	float MagnetInitialSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Magnet")
	float MagnetAcceleration = 800.f;


// Component
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Collision;
	

};
