#include "IndicatorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GetRune/Player/GRPlayer.h"

UIndicatorComponent::UIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(this);
	ArrowMesh->SetRelativeLocation(FVector(100.f, 0.f, 0.f));
}

void UIndicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AGRPlayer* Player = Cast<AGRPlayer>(GetOwner());
	if (!Player) return;

	AActor* NearestEnemy = Player->FindNearestEnemy(EnemySearchRadius);
	if (!NearestEnemy)
	{
		ArrowMesh->SetVisibility(false);
		return;
	}

	ArrowMesh->SetVisibility(true);

	FVector ToEnemy = NearestEnemy->GetActorLocation() - Player->GetActorLocation();
	ToEnemy.Z = 0.f;
	SetWorldRotation(FRotator(0.f, ToEnemy.Rotation().Yaw, 0.f));
}
