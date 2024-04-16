#include "BaseCoin.h"
#include "Kismet/GameplayStatics.h"

// ABaseCoin
//------------------------------------------------------------------------------------------------------------
ABaseCoin::ABaseCoin()
{
	PrimaryActorTick.bCanEverTick = true;
	Was_Collected = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	Coin_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("CoinMesh");
	Coin_Mesh->SetupAttachment(Root);
	Coin_Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Rotation_Rate = 100;
}
//------------------------------------------------------------------------------------------------------------
void ABaseCoin::Tick(float delta_time)
{
	AddActorLocalRotation(FRotator(0, Rotation_Rate * delta_time, 0));
}
//------------------------------------------------------------------------------------------------------------
void ABaseCoin::Play_Custom_Death()
{
	Rotation_Rate = 1500;

	if (Coin_Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Coin_Sound, GetActorLocation());
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Coin sound is not set!"));

	GetWorldTimerManager().SetTimer(Death_Timer_Handle, this, &ABaseCoin::Death_Timer_Complete, 0.5f, false);
}
//------------------------------------------------------------------------------------------------------------
void ABaseCoin::Death_Timer_Complete()
{
	Destroy();
}
//------------------------------------------------------------------------------------------------------------
