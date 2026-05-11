#include "GRBossEnemy.h"
#include "Kismet/KismetSystemLibrary.h"

void AGRBossEnemy::HandleDeath(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator)
{
	Super::HandleDeath(HC, OldValue, NewValue, InInstigator);

	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
