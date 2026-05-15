#include "GRRuneBase.h"
#include "NiagaraComponent.h"

AGRRuneBase::AGRRuneBase()
{
	RuneMesh = CreateDefaultSubobject<UStaticMeshComponent>("RuneMesh");
	RuneMesh->SetupAttachment(Mesh);

	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>("NiagaraEffect");
	NiagaraEffect->SetupAttachment(RuneMesh);
}

void AGRRuneBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGRRuneBase::OnPlayerOverlapped()
{
	Super::OnPlayerOverlapped();
}
