#include "Base_Player.h"
#include "Base_Coin.h"
#include "DrawDebugHelpers.h"

// ABasePlayer
//------------------------------------------------------------------------------------------------------------
ABase_Player::ABase_Player()
{
	Counter_Collected_Coin = 0;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Spring_Arm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	RootComponent = Mesh;
	Spring_Arm->SetupAttachment(Mesh);
	Camera->SetupAttachment(Spring_Arm);

	Mesh->SetSimulatePhysics(true);
	Movement_Force = 100000.0f;
	Jump_Impulse = 100000.0f;

	OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);
}
//------------------------------------------------------------------------------------------------------------
void ABase_Player::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->Possess(this);
	}
}
//------------------------------------------------------------------------------------------------------------
void ABase_Player::Move_Up(float value)
{
	FVector ForceToAdd = FVector(1, 0, 0) * Movement_Force * value;
	Mesh->AddForce(ForceToAdd);
}
//------------------------------------------------------------------------------------------------------------
void ABase_Player::Move_Right(float value)
{
	FVector ForceToAdd = FVector(0, 1, 0) * Movement_Force * value;
	Mesh->AddForce(ForceToAdd);
}
//------------------------------------------------------------------------------------------------------------
void ABase_Player::Jump()
{
	if(Check_Jump() )
	{
		Mesh->AddImpulse(FVector(0, 0, Jump_Impulse) );
	}
}
//------------------------------------------------------------------------------------------------------------
void ABase_Player::Collect_Coin(ABase_Coin* coin)
{
	UE_LOG(LogTemp, Warning, TEXT("[%S] >>>>> other_actor name \"%s\""), __FUNCTION__, *coin->GetActorLabel());

	coin->Was_Collected = true;
	Counter_Collected_Coin++;
	coin->Play_Custom_Death();
	Counter_Collected_Coin_Changed.Broadcast(Counter_Collected_Coin);
}
//------------------------------------------------------------------------------------------------------------
bool ABase_Player::Check_Jump()
{
	FVector start_loc = GetActorLocation();
	FVector world_up = FVector(0, 0, 1);  // Вектор вертикальной  относительно мира, а не персонажа
	FVector end_loc = start_loc + world_up * -51.0f;

	// Дебаг для отрисовки луча
	//DrawDebugLine(GetWorld(), start_loc, end_loc, FColor::Red, true, 0.1f, 0, 1.0f);

	FHitResult hit_result;

	return GetWorld()->LineTraceSingleByChannel(hit_result, start_loc, end_loc, ECC_PhysicsBody);
}
//------------------------------------------------------------------------------------------------------------
void ABase_Player::OnOverlap_Implementation(AActor* overlapped_actor, AActor* other_actor)
{
	if (other_actor != Cast<ABase_Coin>(other_actor) )
		return;

	ABase_Coin* coin = Cast<ABase_Coin>(other_actor);
	if (!coin->Was_Collected)
	{
		if (coin)
		{
			Collect_Coin(coin);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
