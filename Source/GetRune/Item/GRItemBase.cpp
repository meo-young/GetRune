#include "GRItemBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GetRune/GetRune.h"
#include "GetRune/Subsystem/GRObjectPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"

AGRItemBase::AGRItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 메시 컴포넌트를 루트로 설정하고 물리/충돌을 비활성화합니다.
	{
		Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		SetRootComponent(Mesh);
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		Mesh->SetEnableGravity(false);
		Mesh->SetSimulatePhysics(false);
	}

	// 픽업 판정용 박스 콜리전을 메시에 부착합니다.
	{
		Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
		Collision->SetupAttachment(Mesh);
		Collision->SetCollisionProfileName(TEXT("Pickupable"));
	}
}

void AGRItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 자석 인력이 활성화된 경우에만 이동 로직을 수행합니다.
	if (bIsAttracting)
	{
		MagnetAttract(DeltaTime);
	}
}

void AGRItemBase::OnMagnetOverlapped()
{
	// 자석 범위에 진입하면 인력을 시작하고 초기 속도를 설정합니다.
	bIsAttracting = true;
	CurrentMagnetSpeed = MagnetInitialSpeed;
}

void AGRItemBase::OnPlayerOverlapped()
{
	// 플레이어가 아이템을 획득하면 오브젝트 풀에 반납합니다.
	UGRObjectPoolSubsystem* Pool = GetGameInstance()->GetSubsystem<UGRObjectPoolSubsystem>();
	Pool->ReleaseActor(this);
}

void AGRItemBase::MagnetAttract(const float InDeltaTime)
{
	// 캐릭터에 대한 참조를 받아옵니다.
	const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);

	// 시간이 지남에 따라 가속이 붙습니다.
	CurrentMagnetSpeed += MagnetAcceleration * InDeltaTime;

	// 보간을 통해 부드럽게 이동합니다.
	const FVector TargetLocation = Player->GetActorLocation();
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, InDeltaTime, CurrentMagnetSpeed);
	SetActorLocation(NewLocation, true);
}

void AGRItemBase::OnActivated()
{
	// 풀에서 꺼낼 때 자석 상태를 초기화합니다.
	CurrentMagnetSpeed = 0;
	bIsAttracting = false;
}

void AGRItemBase::OnDeactivated()
{
	// 풀에 반납할 때 자석 상태를 초기화합니다.
	CurrentMagnetSpeed = 0;
	bIsAttracting = false;
}
