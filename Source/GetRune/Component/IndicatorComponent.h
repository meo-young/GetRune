#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "IndicatorComponent.generated.h"

class UStaticMeshComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GETRUNE_API UIndicatorComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UIndicatorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


// Component
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ArrowMesh;


// Member Variable
private:
	UPROPERTY(EditAnywhere, Category = "변수|인디케이터")
	float EnemySearchRadius = 2000.f;

};
