#include "Robot_Orb.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// ABasePlayer
//------------------------------------------------------------------------------------------------------------
ARobot_Orb::ARobot_Orb()
{
	Body_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Body_Mesh");
	Head_Spring_Arm = CreateDefaultSubobject<USpringArmComponent>("Head_Spring_Arm");
	Head_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Head_Mesh");
	Camera_Spring_Arm = CreateDefaultSubobject<USpringArmComponent>("Camera_Spring_Arm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Head_Flashlight = CreateDefaultSubobject<USpotLightComponent>("Head_Flashlight");

	RootComponent = Body_Mesh;
	Head_Spring_Arm->SetupAttachment(Body_Mesh);
	Head_Mesh->SetupAttachment(Head_Spring_Arm);
	Head_Flashlight->SetupAttachment(Head_Mesh);
	Camera_Spring_Arm->SetupAttachment(Body_Mesh);
	Camera->SetupAttachment(Camera_Spring_Arm);

	Body_Mesh->SetSimulatePhysics(true);

	Camera_Distance = 1050.0f;
	Target_Camera_Distance = Camera_Distance;
	Camera_Spring_Arm->TargetArmLength = Camera_Distance;

	Movement_Force = 100000.0f;
	Jump_Impulse = 100000.0f;
	Head_Lean_Speed = 7.0f;
	Angle_Of_Head_Lean = 20.0f;

}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->Possess(this);
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	Interp_Angle_Of_Head_Lean_Forward_Backward = FMath::FInterpTo(Interp_Angle_Of_Head_Lean_Forward_Backward, Target_Angle_Of_Head_Lean_Forward_Backward, delta_seconds, Head_Lean_Speed);
	Interp_Angle_Of_Head_Lean_Left_Right = FMath::FInterpTo(Interp_Angle_Of_Head_Lean_Left_Right, Target_Angle_Of_Head_Lean_Left_Right, delta_seconds, Head_Lean_Speed);

	if (Camera_Distance != Target_Camera_Distance)
	{

		Camera_Distance = FMath::FInterpTo(Camera_Distance, Target_Camera_Distance, delta_seconds, 2.0f);
		Camera_Spring_Arm->TargetArmLength = Camera_Distance;
	}

	Turn_Camera();
	Set_Turn_Head_For_Camera();
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Forward_Triggered(bool is_button_pressed)
{
	Is_Moving_Forward = is_button_pressed;

	Move_For_Axis_Triggered(true, false);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Forward_Completed(bool is_button_pressed)
{
	Is_Moving_Forward = is_button_pressed;

	Move_Button_Completed(false, Is_Moving_Backward, Target_Angle_Of_Head_Lean_Forward_Backward);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Backward_Triggered(bool is_button_pressed)
{
	Is_Moving_Backward = is_button_pressed;

	Move_For_Axis_Triggered(true, true);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Backward_Completed(bool is_button_pressed)
{
	Is_Moving_Backward = is_button_pressed;

	Move_Button_Completed(true, Is_Moving_Forward, Target_Angle_Of_Head_Lean_Forward_Backward);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Left_Triggered(bool is_button_pressed)
{
	Is_Moving_Left = is_button_pressed;

	Move_For_Axis_Triggered(false, true);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Left_Completed(bool is_button_pressed)
{
	Is_Moving_Left = is_button_pressed;

	Move_Button_Completed(true, Is_Moving_Right, Target_Angle_Of_Head_Lean_Left_Right);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Right_Triggered(bool is_button_pressed)
{
	Is_Moving_Right = is_button_pressed;

	Move_For_Axis_Triggered(false, false);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Right_Completed(bool is_button_pressed)
{
	Is_Moving_Right = is_button_pressed;

	Move_Button_Completed(false, Is_Moving_Left, Target_Angle_Of_Head_Lean_Left_Right);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Jump()
{
	if(Check_Jump() )
	{
		Body_Mesh->AddImpulse(FVector(0, 0, Jump_Impulse) );
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Zoom_Increase()
{
	Target_Camera_Distance = FMath::Clamp(Target_Camera_Distance + 100.0f, 550.0f, 1350.0f);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Zoom_Decrease()
{
	Target_Camera_Distance = FMath::Clamp(Target_Camera_Distance - 100.0f, 550.0f, 1350.0f);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Turn_On_Off_Fleshlight()
{
	Is_Flashlight_Turn_On = !Is_Flashlight_Turn_On;

	if(Is_Flashlight_Turn_On)
	{
		Head_Flashlight->SetVisibility(true);
	}
	else
	{
		Head_Flashlight->SetVisibility(false);
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Turn_Camera()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		float DeltaMouseX;
		float DeltaMouseY;
		PlayerController->GetInputMouseDelta(DeltaMouseX, DeltaMouseY);

		if (DeltaMouseX != 0.0f)
		{
			FRotator DeltaRotationX(0.0f, DeltaMouseX, 0.0f);

			Camera_Spring_Arm->AddRelativeRotation(DeltaRotationX);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Set_Turn_Head_For_Camera()
{
	FRotator camera_rotation = Camera_Spring_Arm->GetRelativeRotation();

	if (camera_rotation.Yaw < 0.0f)
	{
		Head_Rotation_Angle = 360.0f - (camera_rotation.Yaw * -1.0f);
		FRotator new_rotation = Head_Mesh->GetRelativeRotation();
		new_rotation.Yaw = Head_Rotation_Angle;
		Head_Mesh->SetRelativeRotation(new_rotation);
	}
	else
	{
		Head_Rotation_Angle = camera_rotation.Yaw;
		FRotator new_rotation = Head_Mesh->GetRelativeRotation();
		new_rotation.Yaw = Head_Rotation_Angle;
		Head_Mesh->SetRelativeRotation(new_rotation);
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_For_Axis_Triggered(bool is_forward_backward, bool is_negative_axis)
{
	if (is_forward_backward)
	{
		FVector camera_vector = Camera->GetForwardVector();
		FVector body_vector = Body_Mesh->GetForwardVector();

		FVector com_vector = FVector(camera_vector.X, camera_vector.Y, body_vector.Z);

		if(!is_negative_axis)
		{
			Body_Mesh->AddForce(com_vector * Movement_Force);

			if(Is_Moving_Backward)
				Target_Angle_Of_Head_Lean_Forward_Backward = 0;
			else
				Target_Angle_Of_Head_Lean_Forward_Backward = Angle_Of_Head_Lean;
		}
		else
		{
			Body_Mesh->AddForce(com_vector * Movement_Force * -1);

			if (Is_Moving_Forward)
				Target_Angle_Of_Head_Lean_Forward_Backward = 0;
			else
			Target_Angle_Of_Head_Lean_Forward_Backward = -Angle_Of_Head_Lean;
		}
	}
	else
	{
		FVector camera_vector = Camera->GetRightVector();
		FVector body_vector = Body_Mesh->GetRightVector();

		FVector com_vector = FVector(camera_vector.X, camera_vector.Y, body_vector.Z);

		if (!is_negative_axis)
		{
			Body_Mesh->AddForce(com_vector * Movement_Force);

			if (Is_Moving_Left)
				Target_Angle_Of_Head_Lean_Left_Right = 0;
			else
				Target_Angle_Of_Head_Lean_Left_Right = Angle_Of_Head_Lean;
		}
		else
		{
			Body_Mesh->AddForce(com_vector * Movement_Force * -1);

			if (Is_Moving_Right)
				Target_Angle_Of_Head_Lean_Left_Right = 0;
			else
				Target_Angle_Of_Head_Lean_Left_Right = -Angle_Of_Head_Lean;
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Button_Completed(bool negative_axis_completed, bool opposite_key_triggered, float& changed_axis)
{
	if (negative_axis_completed)
	{
		if (opposite_key_triggered)
			changed_axis = Angle_Of_Head_Lean;
		else
			changed_axis = 0;
	}
	else
	{
		if (opposite_key_triggered)
			changed_axis = -Angle_Of_Head_Lean;
		else
			changed_axis = 0;
	}
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Orb::Check_Jump()
{
	FVector start_loc = Body_Mesh->GetComponentLocation();
	FVector world_up = FVector(0, 0, 1);  // Вектор вертикальной  относительно мира, а не персонажа
	FVector end_loc = start_loc + world_up * -65.0f;

	// Дебаг для отрисовки луча
	//DrawDebugLine(GetWorld(), start_loc, end_loc, FColor::Red, true, 0.1f, 0, 1.0f);

	FHitResult hit_result;
	FCollisionQueryParams query_params;
	query_params.AddIgnoredActor(this);

	return GetWorld()->LineTraceSingleByChannel(hit_result, start_loc, end_loc, ECC_PhysicsBody, query_params);
}
//------------------------------------------------------------------------------------------------------------
