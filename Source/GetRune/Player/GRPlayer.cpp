#include "GRPlayer.h"
#include "GetRune/Item/GRItemBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GRPlayerState.h"
#include "GetRune/Input/GRInputComponent.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputMappingContext.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GetRune/GetRune.h"
#include "GetRune/GRGameplayTags.h"
#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GRPlayerCameraManager.h"
#include "GetRune/Character/GRCharacterMovementComponent.h"
#include "GetRune/Data/SkillInfo.h"
#include "GetRune/GameState/GRGameState.h"
#include "GetRune/Component/HealthComponent.h"
#include "GetRune/Item/Rune/GRRuneBase.h"
#include "GetRune/Spawner/RuneSpawner.h"
#include "GetRune/Data/RuneInfo.h"
#include "GetRune/Subsystem/GRDataTableSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MotionWarpingComponent.h"
#include "GetRune/Component/IndicatorComponent.h"
#include "GetRune/Enemy/GREnemy.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "GetRune/Projectile/GRProjectilePiercing.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"
#include "GetRune/Subsystem/SoundSubsystem.h"

AGRPlayer::AGRPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 이동 관련 설정
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxAcceleration = 4096.0f;
	}
	
	// CapsuleComponent 설정
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	}
	
	// SpringArmComponent 설정
	{
		SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
		SpringArmComponent->SetupAttachment(RootComponent);
		SpringArmComponent->SetUsingAbsoluteRotation(true);
		SpringArmComponent->TargetArmLength = 1600.0f;
		SpringArmComponent->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	}
	
	// CameraComponent 설정
	{
		CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetupAttachment(SpringArmComponent);
	}
	
	// MagnetCollisionComponent 설정
	{
		MagnetCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MagnetCollision"));
		MagnetCollision->SetupAttachment(GetMesh());
		MagnetCollision->SetCollisionProfileName(TEXT("Magnet"));
	}
	
	// IndicatorComponent 설정
	{
		IndicatorComponent = CreateDefaultSubobject<UIndicatorComponent>(TEXT("IndicatorComponent"));
		IndicatorComponent->SetupAttachment(GetMesh());
	}
}


void AGRPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 플레이어 카메라 매니저 캐싱
	if (APlayerController* PC = GetController<APlayerController>())
	{
		PCM = Cast<AGRPlayerCameraManager>(PC->PlayerCameraManager);
	}
	
	// Collision Overlap 이벤트에 함수를 바인딩합니다.
	MagnetCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnMagnetBeginOverlap);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnPlayerBeginOverlap);
	
	// 캐릭터의 기본 수치를 초기화합니다.
	const UGRDataTableSubsystem* DTS = GetGameInstance()->GetSubsystem<UGRDataTableSubsystem>();
	const FCharacterInfo* CharacterInfo = DTS->GetCharacterInfo(GetClass());
	GetCharacterMovement()->MaxWalkSpeed = CharacterInfo->MoveSpeed;
	MagnetCollision->SetSphereRadius(CharacterInfo->MagnetRadius);
	RequiredRuneCount = CharacterInfo->RequiredRuneCount;
	OnRuneCountChanged.Broadcast(0, RequiredRuneCount, GetDominantRuneType());

	for (const FRuneSkillEntry* Entry : { &CharacterInfo->RuneSkillData_1,
										   &CharacterInfo->RuneSkillData_2,
										   &CharacterInfo->RuneSkillData_3 })
	{
		CachedSkillData.Add(Entry->RuneType, Entry->SkillTierData);
		RuneCounts.Add(Entry->RuneType, 0);
		RuneLastAcquired.Add(Entry->RuneType, 0);
	}
	
	RuneSpawner = GetWorld()->GetGameState<AGRGameState>()->RuneSpawnManager;

	if (UGRAbilitySystemComponent* ASC = Cast<UGRAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		HealthComponent->InitializeWithAbilitySystem(ASC);

		// PlayerState의 ASC가 레벨 리로드를 넘어 유지될 때, 이전 사망에서 설정된
		// 상태 태그가 남아 GetMaxSpeed()를 0으로 묶는 것을 방어합니다.
		ASC->SetLooseGameplayTagCount(GRGameplayTags::Status_Death, 0);
		ASC->SetLooseGameplayTagCount(GRGameplayTags::Gameplay_MovementStopped, 0);
	}
}


void AGRPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);
	
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	
	Subsystem->ClearAllMappings();
	
	if (const UInputMappingContext* IMC = DefaultMappingContext.LoadSynchronous())
	{
		// 키 재매핑이 가능하도록 등록합니다.
		if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
		{
			Settings->RegisterInputMappingContext(IMC);
		}
							
		FModifyContextOptions Options = {};
		Options.bIgnoreAllPressedKeysUntilRelease = false;
							
		// 우선순위가 높은 입력 매핑이 낮은 입력 매핑보다 우선적으로 처리됩니다.
		Subsystem->AddMappingContext(IMC, 0, Options);
	}
	
	UGRInputComponent* IC = CastChecked<UGRInputComponent>(PlayerInputComponent);
	
	//IC->BindAbilityAction(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased);
	IC->BindNativeAction(InputConfig, GRGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	IC->BindNativeAction(InputConfig, GRGameplayTags::InputTag_Move, ETriggerEvent::Completed, this, &ThisClass::Input_MoveCompleted);
	
}


UAbilitySystemComponent* AGRPlayer::GetAbilitySystemComponent() const
{
	if (const AGRPlayerState* PS = GetPlayerState<AGRPlayerState>())
	{
		return PS->GetAbilitySystemComponent();	
	}
	
	return Super::GetAbilitySystemComponent();
}


void AGRPlayer::Input_Move(const FInputActionValue& InputActionValue)
{
	if (!Controller) return;

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (ASC->HasMatchingGameplayTag(GRGameplayTags::Gameplay_MovementStopped))
		{
			return;
		}
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	if (Value.X != 0.0f)
	{
		AddMovementInput(MovementRotation.RotateVector(FVector::RightVector), Value.X);
	}
	if (Value.Y != 0.0f)
	{
		AddMovementInput(MovementRotation.RotateVector(FVector::ForwardVector), Value.Y);
	}
}


bool AGRPlayer::AddRune(ERuneType RuneType)
{
	if (TotalRuneCount >= RequiredRuneCount) return false;
	
	USoundSubsystem::Get(this).PlaySFXByName(FName("GetEnergy"));

	int32* Count = RuneCounts.Find(RuneType);
	if (!Count) return false;

	++(*Count);
	++TotalRuneCount;
	RuneLastAcquired[RuneType] = TotalRuneCount;
	OnRuneCountChanged.Broadcast(TotalRuneCount, RequiredRuneCount, GetDominantRuneType());
	return true;
}


void AGRPlayer::Attack()
{
	const int32 Tier = GetCurrentTier();
	const ERuneType DominantType = GetDominantRuneType();

	// 발동할 스킬 정보를 탐색합니다.
	const FSkillTierData* TierData = CachedSkillData.Find(DominantType);
	if (!TierData) return;

	USkillInfo* SkillInfo = nullptr;
	switch (Tier)
	{
		case 1:  SkillInfo = TierData->SkillTier_1; break;
		case 2:  SkillInfo = TierData->SkillTier_2; break;
		default: SkillInfo = TierData->SkillTier_3; break;
	}
	if (!SkillInfo) return;

	CurrentDamage = SkillInfo->EnergyPerDamage * TotalRuneCount;
	CurrentSkillInfo = SkillInfo;
	LastAttackTier = Tier;

	if (const URuneInfo* RuneInfo = RuneSpawner->GetRuneData())
	{
		if (const FRuneClassData* RuneClassData = RuneInfo->RuneClass.Find(DominantType))
		{
			CurrentAttackEffect = RuneClassData->AttackEffect;
		}
	}

	// 소유한 룬의 개수를 초기화합니다.
	for (auto& Pair : RuneCounts) Pair.Value = 0;
	for (auto& Pair : RuneLastAcquired) Pair.Value = 0;
	TotalRuneCount = 0;
	OnRuneCountChanged.Broadcast(0, RequiredRuneCount, GetDominantRuneType());

	FireSkill();
}


void AGRPlayer::FireSkill()
{
	if (!CurrentSkillInfo || !CurrentSkillInfo->AttackMontage) return;

	if (CurrentAttackEffect)
	{
		const float Scale = 1.f + (LastAttackTier - 1) * 0.5f;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CurrentAttackEffect,
			GetActorLocation(), FRotator::ZeroRotator, FVector(Scale));
	}

	if (AActor* NearestEnemy = FindNearestEnemy(EnemySearchRadius))
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromComponent(
			TEXT("AttackTarget"), NearestEnemy->GetRootComponent(), NAME_None, true);
	}

	PlayAnimMontage(CurrentSkillInfo->AttackMontage);
}


void AGRPlayer::LaunchProjectile()
{
	if (!CurrentSkillInfo) return;

	// 발사 방향 결정
	FVector LaunchDirection = GetActorForwardVector();
	if (AActor* NearestEnemy = FindNearestEnemy(EnemySearchRadius))
	{
		FVector ToEnemy = NearestEnemy->GetActorLocation() - GetActorLocation();
		ToEnemy.Z = 0.f;
		LaunchDirection = ToEnemy.GetSafeNormal();
	}

	// 발사 이펙트 재생
	if (CurrentSkillInfo->MuzzleEffect.Effect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CurrentSkillInfo->MuzzleEffect.Effect,
			GetActorLocation(), LaunchDirection.Rotation());
	}
	if (CurrentSkillInfo->MuzzleEffect.Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CurrentSkillInfo->MuzzleEffect.Sound, GetActorLocation());
	}

	// 투사체 스폰 및 발사
	UGRObjectPoolSubsystem* OPS = GetGameInstance()->GetSubsystem<UGRObjectPoolSubsystem>();
	if (!OPS) return;

	AGRProjectilePiercing* Projectile = Cast<AGRProjectilePiercing>(OPS->AcquireActor(
		ProjectileClass, GetActorLocation(), LaunchDirection.Rotation()));
	if (!Projectile) return;

	Projectile->Launch(LaunchDirection, CurrentDamage,
		CurrentSkillInfo->SkillSpeed, CurrentSkillInfo->DamageEffect, GetAbilitySystemComponent(), CurrentSkillInfo);


	PCM->PlayCameraShake(CurrentSkillInfo->CameraShakeScale, CurrentSkillInfo->CameraShakeDuration);
}


AActor* AGRPlayer::FindNearestEnemy(float Radius) const
{
	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(),
		Radius, TArray<TEnumAsByte<EObjectTypeQuery>>(), AGREnemy::StaticClass(),
		TArray<AActor*>(), OverlappedActors);

	AActor* NearestEnemy = nullptr;
	float MinDistSq = FLT_MAX;
	for (AActor* Actor : OverlappedActors)
	{
		const float DistSq = FVector::DistSquared(GetActorLocation(), Actor->GetActorLocation());
		if (DistSq < MinDistSq)
		{
			MinDistSq = DistSq;
			NearestEnemy = Actor;
		}
	}
	return NearestEnemy;
}


int32 AGRPlayer::GetCurrentTier() const
{
	const int32 Tier1Max = RequiredRuneCount / 3;
	const int32 Tier2Max = RequiredRuneCount * 2 / 3;

	if (TotalRuneCount <= Tier1Max) return 1;
	if (TotalRuneCount <= Tier2Max) return 2;
	return 3;
}


ERuneType AGRPlayer::GetDominantRuneType() const
{
	if (RuneCounts.IsEmpty()) return ERuneType::Red;

	// 최다 개수 탐색
	int32 MaxCount = 0;
	for (const auto& Pair : RuneCounts)
	{
		MaxCount = FMath::Max(MaxCount, Pair.Value);
	}

	// 최다 타입 중 가장 최근에 획득한 타입 선택
	int32 LatestOrder = -1;
	ERuneType DominantType = RuneCounts.begin()->Key;
	for (const auto& Pair : RuneCounts)
	{
		if (Pair.Value == MaxCount)
		{
			const int32 Order = RuneLastAcquired[Pair.Key];
			if (Order > LatestOrder)
			{
				LatestOrder = Order;
				DominantType = Pair.Key;
			}
		}
	}

	return DominantType;
}


void AGRPlayer::OnHealthChanged(UHealthComponent* HC, float OldValue, float NewValue, AActor* InInstigator)
{
	Super::OnHealthChanged(HC, OldValue, NewValue, InInstigator);

	if (NewValue < OldValue)
	{
		USoundSubsystem::Get(this).PlaySFXByName(FName("PlayerHit"));
	}
}


void AGRPlayer::Input_MoveCompleted(const FInputActionValue& InputActionValue)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (ASC->HasMatchingGameplayTag(GRGameplayTags::Gameplay_MovementStopped))
		{
			return;
		}
	}

	if (TotalRuneCount > 0)
	{
		Attack();
	}
}


void AGRPlayer::OnMagnetBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGRRuneBase* Rune = Cast<AGRRuneBase>(OtherActor))
	{
		if (TotalRuneCount >= RequiredRuneCount) return;
			
		Rune->OnMagnetOverlapped();
	}
}


void AGRPlayer::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGRRuneBase* Rune = Cast<AGRRuneBase>(OtherActor))
	{
		if (TotalRuneCount >= RequiredRuneCount) return;
			
		AddRune(Rune->RuneType);
		Rune->OnPlayerOverlapped();
		--RuneSpawner->SpawnedCount;
	}
}
