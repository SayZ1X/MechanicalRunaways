#include "BaseCoin.h"
#include "BasePlayer.h"

// ABaseCoin
//------------------------------------------------------------------------------------------------------------
ABaseCoin::ABaseCoin()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>("CoinMesh");
	CoinMesh->SetupAttachment(Root);
	CoinMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RotationRate = 100;

	OnActorBeginOverlap.AddDynamic(this, &ABaseCoin::OnOverlap);
}
//------------------------------------------------------------------------------------------------------------
void ABaseCoin::Tick(float DeltaTime)
{
	AddActorLocalRotation(FRotator(0, RotationRate * DeltaTime, 0));
}
//------------------------------------------------------------------------------------------------------------
void ABaseCoin::PlayCustomDeath()
{
	RotationRate = 1500;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ABaseCoin::DeathTimerComplete, 0.5f, false);
}
//------------------------------------------------------------------------------------------------------------
void ABaseCoin::BeginPlay()
{
	Super::BeginPlay();

}
//------------------------------------------------------------------------------------------------------------
void ABaseCoin::DeathTimerComplete()
{
	Destroy();
}
//------------------------------------------------------------------------------------------------------------
void ABaseCoin::OnOverlap_Implementation(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<ABasePlayer>(OtherActor) != nullptr)
	{
		Destroy();
	}
}
//------------------------------------------------------------------------------------------------------------
