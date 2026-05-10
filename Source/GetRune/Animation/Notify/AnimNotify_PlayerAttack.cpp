#include "AnimNotify_PlayerAttack.h"
#include "NiagaraFunctionLibrary.h"
#include "GetRune/Data/SkillInfo.h"
#include "GetRune/Enemy/GREnemy.h"
#include "GetRune/Player/GRPlayer.h"
#include "GetRune/Projectile/GRProjectilePiercing.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"

void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AGRPlayer* Player = Cast<AGRPlayer>(MeshComp->GetOwner());
	if (!Player) return;

	USkillInfo* SkillInfo = Player->GetCurrentSkillInfo();
	if (!SkillInfo) return;

	// 가장 가까운 적 탐색
	FVector LaunchDirection = Player->GetActorForwardVector();
	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(Player->GetWorld(), Player->GetActorLocation(),
		EnemySearchRadius, TArray<TEnumAsByte<EObjectTypeQuery>>(), AGREnemy::StaticClass(),
		TArray<AActor*>(), OverlappedActors);

	if (OverlappedActors.Num() > 0)
	{
		AActor* NearestEnemy = nullptr;
		float MinDistSq = FLT_MAX;
		for (AActor* Actor : OverlappedActors)
		{
			const float DistSq = FVector::DistSquared(Player->GetActorLocation(), Actor->GetActorLocation());
			if (DistSq < MinDistSq)
			{
				MinDistSq = DistSq;
				NearestEnemy = Actor;
			}
		}
		if (NearestEnemy)
		{
			FVector ToEnemy = NearestEnemy->GetActorLocation() - Player->GetActorLocation();
			ToEnemy.Z = 0.f;
			LaunchDirection = ToEnemy.GetSafeNormal();
		}
	}

	// 발사 이펙트 재생
	if (SkillInfo->MuzzleEffect.Effect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Player->GetWorld(), SkillInfo->MuzzleEffect.Effect,
			Player->GetActorLocation(), LaunchDirection.Rotation());
	}
	if (SkillInfo->MuzzleEffect.Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(Player, SkillInfo->MuzzleEffect.Sound, Player->GetActorLocation());
	}

	// 투사체 스폰
	UGRObjectPoolSubsystem* OPS = UGameplayStatics::GetGameInstance(Player)->GetSubsystem<UGRObjectPoolSubsystem>();
	if (!OPS) return;

	AGRProjectilePiercing* Projectile = Cast<AGRProjectilePiercing>(OPS->AcquireActor(
		ProjectileClass, Player->GetActorLocation(), LaunchDirection.Rotation()));
	if (!Projectile) return;

	Projectile->Launch(LaunchDirection, Player->GetCurrentDamage(),
		SkillInfo->SkillSpeed, SkillInfo->DamageEffect, Player->GetAbilitySystemComponent(), SkillInfo);
}
