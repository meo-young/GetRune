#include "GRRuneBase.h"

AGRRuneBase::AGRRuneBase()
{
	RuneMesh = CreateDefaultSubobject<UStaticMeshComponent>("RuneMesh");
	RuneMesh->SetupAttachment(Mesh);
}

void AGRRuneBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGRRuneBase::OnPlayerOverlapped()
{
	Super::OnPlayerOverlapped();
}
