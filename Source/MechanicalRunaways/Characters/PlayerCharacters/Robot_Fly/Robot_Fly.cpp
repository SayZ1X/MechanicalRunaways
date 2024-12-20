#include "Robot_Fly.h"
#include "Engine/Engine.h"

// ARobot_Fly
//------------------------------------------------------------------------------------------------------------
ARobot_Fly::ARobot_Fly()
{
	Robot_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Robot_Mesh");
	Capsule_Component = CreateDefaultSubobject<UCapsuleComponent>("Capsule_Component");
	Camera_Spring_Arm = CreateDefaultSubobject<USpringArmComponent>("Camera_Spring_Arm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Flashlight_Component = CreateDefaultSubobject<USpotLightComponent>("Flashlight_Component");

	RootComponent = Capsule_Component;
	Robot_Mesh->SetupAttachment(Capsule_Component);
	Camera_Spring_Arm->SetupAttachment(Robot_Mesh);
	Camera->SetupAttachment(Camera_Spring_Arm);
	Flashlight_Component->SetupAttachment(Robot_Mesh);

	Movement_Component = CreateDefaultSubobject<UFloatingPawnMovement>("Movement_Component");
	Movement_Component->UpdatedComponent = RootComponent;

	Camera_Distance = 550.0f;
	Target_Camera_Distance = Camera_Distance;
	Camera_Spring_Arm->TargetArmLength = Camera_Distance;

	Body_Lean_Speed = 4.0f;
	Body_Rotation_Angle = 15.0f;

	Blades_Current_Rotation = 0.0f;
	Blades_Rotation_Speed = 1800.0f;

	PrimaryActorTick.bCanEverTick = true;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::BeginPlay()
{
	Super::BeginPlay();

}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	Turn_Camera();
	Change_Camera_Distance(delta_seconds);
	Change_Body_Angle_Lean(delta_seconds);
	Rotate_Blades(delta_seconds);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Forward_Triggered(bool is_button_pressed)
{
	Is_Moving_Forward = is_button_pressed;

	Change_State_If_Moving();
	Move_For_Axis_Triggered(true, false);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Forward_Completed(bool is_button_pressed)
{
	Is_Moving_Forward = is_button_pressed;

	Change_State_If_Moving();
	Move_Button_Completed(false, Is_Moving_Backward, Target_Angle_Of_Body_Lean_Forward_Backward);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Backward_Triggered(bool is_button_pressed)
{
	Is_Moving_Backward = is_button_pressed;

	Change_State_If_Moving();
	Move_For_Axis_Triggered(true, true);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Backward_Completed(bool is_button_pressed)
{
	Is_Moving_Backward = is_button_pressed;

	Change_State_If_Moving();
	Move_Button_Completed(true, Is_Moving_Forward, Target_Angle_Of_Body_Lean_Forward_Backward);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Left_Triggered(bool is_button_pressed)
{
	Is_Moving_Left = is_button_pressed;

	Change_State_If_Moving();
	Move_For_Axis_Triggered(false, true);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Left_Completed(bool is_button_pressed)
{
	Is_Moving_Left = is_button_pressed;

	Change_State_If_Moving();
	Move_Button_Completed(true, Is_Moving_Right, Target_Angle_Of_Body_Lean_Left_Right);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Right_Triggered(bool is_button_pressed)
{
	Is_Moving_Right = is_button_pressed;

	Change_State_If_Moving();
	Move_For_Axis_Triggered(false, false);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Right_Completed(bool is_button_pressed)
{
	Is_Moving_Right = is_button_pressed;

	Change_State_If_Moving();
	Move_Button_Completed(false, Is_Moving_Left, Target_Angle_Of_Body_Lean_Left_Right);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Up_Triggered(bool is_button_pressed)
{
	if (Movement_Component && (Movement_Component->UpdatedComponent == RootComponent))
	{

		Movement_Component->AddInputVector(GetActorUpVector() * 50);
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Down_Triggered(bool is_button_pressed)
{
	if (Movement_Component && (Movement_Component->UpdatedComponent == RootComponent))
	{

		Movement_Component->AddInputVector(GetActorUpVector() * -50);
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Zoom_Increase()
{
	Target_Camera_Distance = FMath::Clamp(Target_Camera_Distance + 100.0f, 250.0f, 850.0f);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Zoom_Decrease()
{
	Target_Camera_Distance = FMath::Clamp(Target_Camera_Distance - 100.0f, 250.0f, 850.0f);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Turn_On_Off_Fleshlight()
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
void ARobot_Fly::Turn_Camera()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		float DeltaMouseX;
		float DeltaMouseY;
		PlayerController->GetInputMouseDelta(DeltaMouseX, DeltaMouseY);

		if (DeltaMouseX != 0.0f || DeltaMouseY != 0.0f)
		{

			float MinPitch = -50.0f;
			float MaxPitch = 30.0f;
			
			FRotator CurrentRotation = Camera_Spring_Arm->GetRelativeRotation();

			float NewPitch = CurrentRotation.Pitch + DeltaMouseY;

			NewPitch = FMath::Clamp(NewPitch, MinPitch, MaxPitch);

			FRotator NewRotation(NewPitch, CurrentRotation.Yaw + DeltaMouseX, 0.0f);
			Camera_Spring_Arm->SetRelativeRotation(NewRotation);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_For_Axis_Triggered(bool is_forward_backward, bool is_negative_axis)
{
	if (Movement_Component && (Movement_Component->UpdatedComponent == RootComponent))
	{
		if (is_forward_backward)
		{
			if (!is_negative_axis)
			{
				FVector Direction = Camera_Spring_Arm->GetForwardVector() * 100;
				Direction.Z = 0.0f;

				if (Is_Moving_Backward)
					Target_Angle_Of_Body_Lean_Forward_Backward = 0;
				else
					Target_Angle_Of_Body_Lean_Forward_Backward = Body_Rotation_Angle;

				Movement_Component->AddInputVector(Direction);
			}
			else
			{
				FVector Direction = Camera_Spring_Arm->GetForwardVector() * -100;
				Direction.Z = 0.0f;

				if (Is_Moving_Forward)
					Target_Angle_Of_Body_Lean_Forward_Backward = 0;
				else
					Target_Angle_Of_Body_Lean_Forward_Backward = -Body_Rotation_Angle;

				Movement_Component->AddInputVector(Direction);
			}
		}
		else
		{
			if (!is_negative_axis)
			{
				FVector Direction = Camera_Spring_Arm->GetRightVector() * 100;
				Direction.Z = 0.0f;

				if (Is_Moving_Left)
					Target_Angle_Of_Body_Lean_Left_Right = 0;
				else
					Target_Angle_Of_Body_Lean_Left_Right = Body_Rotation_Angle;

				Movement_Component->AddInputVector(Direction);
			}
			else
			{
				FVector Direction = Camera_Spring_Arm->GetRightVector() * -100;
				Direction.Z = 0.0f;

				if(Is_Moving_Right)
					Target_Angle_Of_Body_Lean_Left_Right = 0;
				else
					Target_Angle_Of_Body_Lean_Left_Right = -Body_Rotation_Angle;

				Movement_Component->AddInputVector(Direction);
			}
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Button_Completed(bool negative_axis_completed, bool opposite_key_triggered, float& changed_axis)
{
	if (negative_axis_completed)
	{
		if (opposite_key_triggered)
			changed_axis = Angle_Of_Body_Lean;
		else
			changed_axis = 0;
	}
	else
	{
		if (opposite_key_triggered)
			changed_axis = -Angle_Of_Body_Lean;
		else
			changed_axis = 0;
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Change_State_If_Moving()
{
	if(Is_Moving_Forward ||
		Is_Moving_Backward ||
		Is_Moving_Left ||
		Is_Moving_Right)
	{
		Is_Moving = true;
	}
	else
	{
		Is_Moving = false;
	}
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Fly::Get_Is_Moving()
{
	return Is_Moving;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Rotate_Blades(float delta_seconds)
{
	Blades_Current_Rotation += Blades_Rotation_Speed * delta_seconds;

	if (Blades_Current_Rotation >= 360.0f)
	{
		Blades_Current_Rotation -= 360.0f;
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Change_Camera_Distance(float delta_seconds)
{
	if (Camera_Distance != Target_Camera_Distance)
	{

		Camera_Distance = FMath::FInterpTo(Camera_Distance, Target_Camera_Distance, delta_seconds, 2.0f);
		Camera_Spring_Arm->TargetArmLength = Camera_Distance;
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Change_Body_Angle_Lean(float delta_seconds)
{
	Interp_Angle_Of_Body_Lean_Forward_Backward = FMath::FInterpTo(Interp_Angle_Of_Body_Lean_Forward_Backward, Target_Angle_Of_Body_Lean_Forward_Backward, delta_seconds, Body_Lean_Speed);
	Interp_Angle_Of_Body_Lean_Left_Right = FMath::FInterpTo(Interp_Angle_Of_Body_Lean_Left_Right, Target_Angle_Of_Body_Lean_Left_Right, delta_seconds, Body_Lean_Speed);
}
//------------------------------------------------------------------------------------------------------------
