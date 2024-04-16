#include "BasePlayer.h"
#include "BaseCoin.h"
#include "DrawDebugHelpers.h"

// ABasePlayer
//------------------------------------------------------------------------------------------------------------
ABasePlayer::ABasePlayer()
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
void ABasePlayer::SetupPlayerInputComponent(UInputComponent* player_input_component)
{
	Super::SetupPlayerInputComponent(player_input_component);
	InputComponent->BindAxis("MoveUp", this, &ABasePlayer::Move_Up);
	InputComponent->BindAxis("MoveRight", this, &ABasePlayer::Move_Right);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ABasePlayer::Jump);
}
//------------------------------------------------------------------------------------------------------------
void ABasePlayer::Move_Up(float value)
{
	FVector ForceToAdd = FVector(1, 0, 0) * Movement_Force * value;
	Mesh->AddForce(ForceToAdd);
}
//------------------------------------------------------------------------------------------------------------
void ABasePlayer::Move_Right(float value)
{
	FVector ForceToAdd = FVector(0, 1, 0) * Movement_Force * value;
	Mesh->AddForce(ForceToAdd);
}
//------------------------------------------------------------------------------------------------------------
void ABasePlayer::Collect_Coin(ABaseCoin* coin)
{
	UE_LOG(LogTemp, Warning, TEXT("[%S] >>>>> other_actor name \"%s\""), __FUNCTION__, *coin->GetActorLabel());

	coin->Was_Collected = true;
	Counter_Collected_Coin++;
	coin->Play_Custom_Death();
	Counter_Collected_Coin_Changed.Broadcast(Counter_Collected_Coin);
}
//------------------------------------------------------------------------------------------------------------
bool ABasePlayer::check_Jump()
{
	FVector start_loc = GetActorLocation();
	FVector world_up = FVector(0, 0, 1);  // Вектор вертикальной  относительно мира, а не персонажа
	FVector end_loc = start_loc + world_up * -51.0f;

	// Дебаг для отрисовки луча
	DrawDebugLine(GetWorld(), start_loc, end_loc, FColor::Red, false, 0.1f, 0, 1.0f);

	FHitResult hit_result;

	return GetWorld()->LineTraceSingleByChannel(hit_result, start_loc, end_loc, ECC_PhysicsBody);
}
//------------------------------------------------------------------------------------------------------------
void ABasePlayer::OnOverlap_Implementation(AActor* overlapped_actor, AActor* other_actor)
{
	if (other_actor != Cast<ABaseCoin>(other_actor) )
		return;

	ABaseCoin* coin = Cast<ABaseCoin>(other_actor);
	if (!coin->Was_Collected)
	{
		if (coin)
		{
			Collect_Coin(coin);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
