#include "BasePlayer.h"
#include "DrawDebugHelpers.h"

// ABasePlayer
//------------------------------------------------------------------------------------------------------------
ABasePlayer::ABasePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	RootComponent = Mesh;
	SpringArm->SetupAttachment(Mesh);
	Camera->SetupAttachment(SpringArm);

	Mesh->SetSimulatePhysics(true);
	MovementForce = 100000;
}
//------------------------------------------------------------------------------------------------------------
void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//------------------------------------------------------------------------------------------------------------
void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("MoveUp", this, &ABasePlayer::MoveUp);
	InputComponent->BindAxis("MoveRight", this, &ABasePlayer::MoveRight);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ABasePlayer::Jump);
}
//------------------------------------------------------------------------------------------------------------
void ABasePlayer::MoveUp(float Value)
{
	FVector ForceToAdd = FVector(1, 0, 0) * MovementForce * Value;
	Mesh->AddForce(ForceToAdd);
}
//------------------------------------------------------------------------------------------------------------
void ABasePlayer::MoveRight(float Value)
{
	FVector ForceToAdd = FVector(0, 1, 0) * MovementForce * Value;
	Mesh->AddForce(ForceToAdd);
}
//------------------------------------------------------------------------------------------------------------
bool ABasePlayer::checkJump()
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
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

}
//------------------------------------------------------------------------------------------------------------
