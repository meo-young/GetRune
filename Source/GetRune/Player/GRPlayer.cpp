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
#include "GetRune/Character/GRCharacterMovementComponent.h"
#include "GetRune/Data/SkillInfo.h"
#include "GetRune/GameState/GRGameState.h"
#include "GetRune/Component/HealthComponent.h"
#include "GetRune/Item/Rune/GRRuneBase.h"
#include "GetRune/Spawner/RuneSpawner.h"
#include "GetRune/Subsystem/GRDataTableSubsystem.h"
#include "Kismet/GameplayStatics.h"

AGRPlayer::AGRPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// CapsuleComponent 설정
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	}
	
	// SpringArmComponent 설정
	{
		SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
		SpringArmComponent->SetupAttachment(RootComponent);
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
}

void AGRPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Collision Overlap 이벤트에 함수를 바인딩합니다.
	MagnetCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnMagnetBeginOverlap);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnPlayerBeginOverlap);
	
	// 캐릭터의 기본 수치를 초기화합니다.
	const UGRDataTableSubsystem* DTS = GetGameInstance()->GetSubsystem<UGRDataTableSubsystem>();
	const FCharacterInfo* CharacterInfo = DTS->GetCharacterInfo(GetClass());
	GetCharacterMovement()->MaxWalkSpeed = CharacterInfo->MoveSpeed;
	MagnetCollision->SetSphereRadius(CharacterInfo->MagnetRadius);
	RequiredRuneCount = CharacterInfo->RequiredRuneCount;

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

	int32* Count = RuneCounts.Find(RuneType);
	if (!Count) return false;

	++(*Count);
	++TotalRuneCount;
	RuneLastAcquired[RuneType] = TotalRuneCount;
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

	// 소유한 룬의 개수를 초기화합니다.
	for (auto& Pair : RuneCounts) Pair.Value = 0;
	for (auto& Pair : RuneLastAcquired) Pair.Value = 0;
	TotalRuneCount = 0;

	FireSkill();
}

void AGRPlayer::FireSkill()
{
	if (CurrentSkillInfo && CurrentSkillInfo->AttackMontage)
	{
		PlayAnimMontage(CurrentSkillInfo->AttackMontage);
	}
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

void AGRPlayer::Input_MoveCompleted(const FInputActionValue& InputActionValue)
{
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
