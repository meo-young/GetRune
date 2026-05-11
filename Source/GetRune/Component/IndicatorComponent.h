#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "IndicatorComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GETRUNE_API UIndicatorComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UIndicatorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "변수|인디케이터")
	float EnemySearchRadius = 2000.f;

};
