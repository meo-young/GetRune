#include "GRProjectilePiercing.h"

void AGRProjectilePiercing::HandleOverlap(AActor* OtherActor)
{
	Super::HandleOverlap(OtherActor);
	// Duration 만료 시 ReturnToPool이 호출되므로 여기서는 처리하지 않습니다.
}
