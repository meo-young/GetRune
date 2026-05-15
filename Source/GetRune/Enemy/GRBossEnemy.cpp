#include "GRBossEnemy.h"
#include "MotionWarpingComponent.h"
#include "GetRune/Data/StageInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void AGRBossEnemy::Attack()
{
	if (!bCanAttack || AttackMontages.IsEmpty()) return;

	UAnimMontage* SelectedMontage = AttackMontages[FMath::RandRange(0, AttackMontages.Num() - 1)];
	if (!SelectedMontage) return;

	if (APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromComponent(
			TEXT("AttackTarget"), Player->GetRootComponent(), NAME_None, true);
	}

	PlayAnimMontage(SelectedMontage);
}

void AGRBossEnemy::OnAttackFinished()
{
	Super::OnAttackFinished();

	bCanAttack = false;
	if (CurrentEnemyInfo)
	{
		GetWorldTimerManager().SetTimer(CooldownHandle, this,
			&AGRBossEnemy::OnAttackCooldownEnd, CurrentEnemyInfo->AttackCooldown, false);
	}
}

void AGRBossEnemy::OnAttackCooldownEnd()
{
	bCanAttack = true;
}

void AGRBossEnemy::HandleDeath(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator)
{
	Super::HandleDeath(HC, OldValue, NewValue, InInstigator);

	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
