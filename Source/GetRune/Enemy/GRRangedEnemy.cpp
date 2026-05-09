#include "GRRangedEnemy.h"
#include "GameFramework/Character.h"
#include "GetRune/Data/StageInfo.h"
#include "GetRune/Projectile/GRProjectile.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"

void AGRRangedEnemy::Attack()
{
	if (!bCanAttack || !AttackMontage) return;

	PlayAnimMontage(AttackMontage);
}

void AGRRangedEnemy::OnAttackFinished()
{
	Super::OnAttackFinished();
	
	bCanAttack = false;
	if (CurrentEnemyInfo)
	{
		GetWorldTimerManager().SetTimer(CooldownHandle, this,
			&AGRRangedEnemy::OnAttackCooldownEnd, CurrentEnemyInfo->AttackCooldown, false);
	}
}

void AGRRangedEnemy::FireProjectile()
{
	const APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!Player || !ProjectileClass || !CurrentEnemyInfo) return;

	const FVector Direction = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	UGRObjectPoolSubsystem* OPS = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UGRObjectPoolSubsystem>();
	AGRProjectile* Projectile = Cast<AGRProjectile>(OPS->AcquireActor(ProjectileClass, GetActorLocation(), Direction.Rotation()));
	if (!Projectile) return;
	Projectile->Launch(Direction, CurrentEnemyInfo->AttackPower, CurrentEnemyInfo->ProjectileSpeed, ContactDamageGE, ASC);
}

void AGRRangedEnemy::OnAttackCooldownEnd()
{
	bCanAttack = true;
}
