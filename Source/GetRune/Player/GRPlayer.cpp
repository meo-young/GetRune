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
#include "GetRune/GetRune.h"
#include "GetRune/GRGameplayTags.h"
#include "GetRune/AbilitySystem/GRAbilitySystemComponent.h"
#include "GetRune/Component/GRRuneSpawnComponent.h"
#include "GetRune/Subsystem/GRDataTableSubsystem.h"

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
	
	// RuneSpawnComponent 설정
	{
		RuneSpawnComponent = CreateDefaultSubobject<UGRRuneSpawnComponent>(TEXT("RuneSpawnComponent"));
	}
}

void AGRPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Collision Overlap 이벤트에 함수를 바인딩합니다.
	MagnetCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnMagnetBeginOverlap);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnPlayerBeginOverlap);
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
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void AGRPlayer::OnMagnetBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPickupable* Pickupable = Cast<IPickupable>(OtherActor))
	{
		Pickupable->OnMagnetOverlapped();
	}
}

void AGRPlayer::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPickupable* Pickupable = Cast<IPickupable>(OtherActor))
	{
		Pickupable->OnPlayerOverlapped();
	}
}
