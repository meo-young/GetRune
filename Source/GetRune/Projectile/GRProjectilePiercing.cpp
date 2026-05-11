#include "GRProjectilePiercing.h"

void AGRProjectilePiercing::OnActivated()
{
	Super::OnActivated();
	HitActors.Reset();
}

void AGRProjectilePiercing::HandleOverlap(AActor* OtherActor)
{
	if (HitActors.Contains(OtherActor)) return;
	HitActors.Add(OtherActor);
	Super::HandleOverlap(OtherActor);
}
