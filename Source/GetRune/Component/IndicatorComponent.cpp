#include "IndicatorComponent.h"
#include "GetRune/Player/GRPlayer.h"

UIndicatorComponent::UIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UIndicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AGRPlayer* Player = Cast<AGRPlayer>(GetOwner());
	if (!Player) return;

	AActor* NearestEnemy = Player->FindNearestEnemy(EnemySearchRadius);
	if (!NearestEnemy)
	{
		SetVisibility(false);
		return;
	}

	SetVisibility(true);

	FVector ToEnemy = NearestEnemy->GetActorLocation() - Player->GetActorLocation();
	ToEnemy.Z = 0.f;
	FVector Direction = ToEnemy.GetSafeNormal();
	SetWorldLocation(Player->GetActorLocation() + Direction * 150.f);
	SetWorldRotation(FRotator(-90.f, ToEnemy.Rotation().Yaw, 0.f));
}
