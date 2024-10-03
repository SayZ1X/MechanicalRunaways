#include "Char_Robot_Fly.h"

// AChar_Robot_Fly
//------------------------------------------------------------------------------------------------------------
AChar_Robot_Fly::AChar_Robot_Fly()
{
	Scene_Component = CreateDefaultSubobject<USceneComponent>("Scene_Component");
	Robot_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Robot_Mesh");
	Capsule_Component = CreateDefaultSubobject<UCapsuleComponent>("Capsule_Component");
	Camera_Spring_Arm = CreateDefaultSubobject<USpringArmComponent>("Camera_Spring_Arm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Flashlight_Component = CreateDefaultSubobject<USpotLightComponent>("Flashlight_Component");

	RootComponent = Scene_Component;
	Robot_Mesh->SetupAttachment(Scene_Component);
	Capsule_Component->SetupAttachment(Robot_Mesh);
	Camera_Spring_Arm->SetupAttachment(Robot_Mesh);
	Camera->SetupAttachment(Camera_Spring_Arm);
	Flashlight_Component->SetupAttachment(Robot_Mesh);

	Movement_Component = CreateDefaultSubobject<UFloatingPawnMovement>("Movement_Component");
	Movement_Component->UpdatedComponent = RootComponent;

	Camera_Distance = 550.0f;
	Target_Camera_Distance = Camera_Distance;
	Camera_Spring_Arm->TargetArmLength = Camera_Distance;

	PrimaryActorTick.bCanEverTick = true;
}
//------------------------------------------------------------------------------------------------------------
void AChar_Robot_Fly::BeginPlay()
{
	Super::BeginPlay();
	
}
//------------------------------------------------------------------------------------------------------------
void AChar_Robot_Fly::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	Turn_Camera();

	if (Camera_Distance != Target_Camera_Distance)
	{

		Camera_Distance = FMath::FInterpTo(Camera_Distance, Target_Camera_Distance, delta_seconds, 2.0f);
		Camera_Spring_Arm->TargetArmLength = Camera_Distance;
	}
}
//------------------------------------------------------------------------------------------------------------
void AChar_Robot_Fly::Move_Forward_Triggered(bool is_button_pressed)
{
	if (Movement_Component && (Movement_Component->UpdatedComponent == RootComponent))
	{

		FVector Direction = Camera_Spring_Arm->GetForwardVector() * 100;
		Direction.Z = 0.0f;

		Movement_Component->AddInputVector(Direction);
	}
}
//------------------------------------------------------------------------------------------------------------
void AChar_Robot_Fly::Move_Backward_Triggered(bool is_button_pressed)
{
	if (Movement_Component && (Movement_Component->UpdatedComponent == RootComponent))
	{

		FVector Direction = Camera_Spring_Arm->GetForwardVector() * -100;
		Direction.Z = 0.0f;

		Movement_Component->AddInputVector(Direction);
	}
}
//------------------------------------------------------------------------------------------------------------
void AChar_Robot_Fly::Move_Right_Triggered(bool is_button_pressed)
{
	if (Movement_Component && (Movement_Component->UpdatedComponent == RootComponent))
	{

		FVector Direction = Camera_Spring_Arm->GetRightVector() * 100;
		Direction.Z = 0.0f;

		Movement_Component->AddInputVector(Direction);
	}
}
//------------------------------------------------------------------------------------------------------------
void AChar_Robot_Fly::Move_Left_Triggered(bool is_button_pressed)
{
	if (Movement_Component && (Movement_Component->UpdatedComponent == RootComponent))
	{
		FVector Direction = Camera_Spring_Arm->GetRightVector() * -100;
		Direction.Z = 0.0f;

		Movement_Component->AddInputVector(Direction);
	}
}
//------------------------------------------------------------------------------------------------------------
void AChar_Robot_Fly::Move_Up_Triggered(bool is_button_pressed)
{
	if (Movement_Component && (Movement_Component->UpdatedComponent == RootComponent))
	{

		Movement_Component->AddInputVector(GetActorUpVector() * 50);
	}
}
//------------------------------------------------------------------------------------------------------------
void AChar_Robot_Fly::Move_Down_Triggered(bool is_button_pressed)
{
	if (Movement_Component && (Movement_Component->UpdatedComponent == RootComponent))
	{

		Movement_Component->AddInputVector(GetActorUpVector() * -50);
	}
}
//------------------------------------------------------------------------------------------------------------
void AChar_Robot_Fly::Zoom_Increase()
{
	Target_Camera_Distance = FMath::Clamp(Target_Camera_Distance + 100.0f, 250.0f, 850.0f);
}
//------------------------------------------------------------------------------------------------------------
void AChar_Robot_Fly::Zoom_Decrease()
{
	Target_Camera_Distance = FMath::Clamp(Target_Camera_Distance - 100.0f, 250.0f, 850.0f);
}
//------------------------------------------------------------------------------------------------------------
void AChar_Robot_Fly::Turn_On_Off_Fleshlight()
{
	Is_Flashlight_Turn_On = !Is_Flashlight_Turn_On;

	if (Is_Flashlight_Turn_On)
	{
		Flashlight_Component->SetVisibility(true);
	}
	else
	{
		Flashlight_Component->SetVisibility(false);
	}
}
//------------------------------------------------------------------------------------------------------------
void AChar_Robot_Fly::Turn_Camera()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		float DeltaMouseX;
		float DeltaMouseY;
		PlayerController->GetInputMouseDelta(DeltaMouseX, DeltaMouseY);

		if (DeltaMouseX != 0.0f || DeltaMouseY != 0.0f)
		{

			float MinPitch = -80.0f;
			float MaxPitch = 80.0f;

			FRotator CurrentRotation = Camera_Spring_Arm->GetRelativeRotation();

			float NewPitch = CurrentRotation.Pitch + DeltaMouseY;

			NewPitch = FMath::Clamp(NewPitch, MinPitch, MaxPitch);

			FRotator NewRotation(NewPitch, CurrentRotation.Yaw + DeltaMouseX, 0.0f);
			Camera_Spring_Arm->SetRelativeRotation(NewRotation);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
