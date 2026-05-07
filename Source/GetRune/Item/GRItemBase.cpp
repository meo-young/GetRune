#include "GRItemBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GetRune/GetRune.h"
#include "Kismet/GameplayStatics.h"

AGRItemBase::AGRItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Mesh 설정
	{
		Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		SetRootComponent(Mesh);
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		Mesh->SetEnableGravity(false);
		Mesh->SetSimulatePhysics(false);
	}

	// Collision 설정
	{
		Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
		Collision->SetupAttachment(Mesh);
		Collision->SetCollisionProfileName(TEXT("Pickupable"));
	}
}

void AGRItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGRItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (!Player)
	{
		SetActorTickEnabled(false);
		return;
	}

	CurrentMagnetSpeed += MagnetAcceleration * DeltaTime;

	const FVector TargetLocation = Player->GetActorLocation();
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaTime, CurrentMagnetSpeed);
	SetActorLocation(NewLocation, true);
}

void AGRItemBase::OnMagnetOverlapped()
{
	StartMagnetAttract();
}

void AGRItemBase::StartMagnetAttract()
{
	CurrentMagnetSpeed = MagnetInitialSpeed;
	SetActorTickEnabled(true);
}

void AGRItemBase::OnPlayerOverlapped()
{
	LOG(TEXT("%s Pick !"), *GetName());
	Destroy();
}
