#include "GRProjectileSingle.h"

void AGRProjectileSingle::HandleOverlap(AActor* OtherActor)
{
	Super::HandleOverlap(OtherActor);
	ReturnToPool();
}
