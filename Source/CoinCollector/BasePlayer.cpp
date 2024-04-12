#include "BasePlayer.h"

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
void ABasePlayer::checkJump()
{
	FVector DownVector = GetActorUpVector() * -300;
}
//------------------------------------------------------------------------------------------------------------
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

}
//------------------------------------------------------------------------------------------------------------
