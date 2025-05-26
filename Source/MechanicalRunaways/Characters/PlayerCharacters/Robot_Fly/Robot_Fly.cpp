#include "Robot_Fly.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

// ARobot_Fly
//------------------------------------------------------------------------------------------------------------
ARobot_Fly::ARobot_Fly()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	Camera_Spring_Arm = CreateDefaultSubobject<USpringArmComponent>("Camera_Spring_Arm");
	Camera_Spring_Arm->SetIsReplicated(true);
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Flashlight_Component = CreateDefaultSubobject<USpotLightComponent>("Flashlight_Component");
	Flashlight_Component->SetIsReplicated(true);

	SetRootComponent(GetCapsuleComponent());
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	Camera_Spring_Arm->SetupAttachment(GetMesh());
	Camera->SetupAttachment(Camera_Spring_Arm);
	Flashlight_Component->SetupAttachment(GetMesh(), TEXT("Flashlight_Socket"));

	Is_Flashlight_Turn_On = false;
	Flashlight_Component->SetVisibility(Is_Flashlight_Turn_On);

	Camera_Distance = 700.0f;
	Target_Camera_Distance = Camera_Distance;
	Camera_Spring_Arm->TargetArmLength = Camera_Distance;

	Movement_Speed = 500.0f;
	GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	GetCharacterMovement()->DefaultWaterMovementMode = EMovementMode::MOVE_Flying;
	GetCharacterMovement()->MaxFlySpeed = 500.0f;
	GetCharacterMovement()->BrakingDecelerationFlying = 250.0f;

	Body_Lean_Speed = 4.0f;
	Body_Rotation_Angle = 15.0f;

	Blades_Current_Rotation = 0.0f;
	Blades_Rotation_Speed = 1800.0f;
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
	Update_Actor_Position_MPC_Value();
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Add properties to replicated for the derived class
	DOREPLIFETIME(ARobot_Fly, Camera_Spring_Arm);
	DOREPLIFETIME(ARobot_Fly, Flashlight_Component);
	DOREPLIFETIME(ARobot_Fly, Body_Rotation_Angle);
	DOREPLIFETIME(ARobot_Fly, Target_Angle_Of_Body_Lean_Forward_Backward);
	DOREPLIFETIME(ARobot_Fly, Target_Angle_Of_Body_Lean_Left_Right);

	DOREPLIFETIME(ARobot_Fly, Is_Moving);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Forward_Started(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	Is_Moving_Forward = value.Get<bool>();

	Change_State_If_Moving();
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Forward_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	Move_For_Axis_Triggered(true, false);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Forward_Completed(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	Is_Moving_Forward = value.Get<bool>();

	Change_State_If_Moving();
	Move_Button_Completed(false, Is_Moving_Backward, Target_Angle_Of_Body_Lean_Forward_Backward);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Backward_Started(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	Is_Moving_Backward = value.Get<bool>();

	Change_State_If_Moving();
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Backward_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	Move_For_Axis_Triggered(true, true);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Backward_Completed(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	Is_Moving_Backward = value.Get<bool>();

	Change_State_If_Moving();
	Move_Button_Completed(true, Is_Moving_Forward, Target_Angle_Of_Body_Lean_Forward_Backward);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Left_Started(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	Is_Moving_Left = value.Get<bool>();

	Change_State_If_Moving();
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Left_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	Move_For_Axis_Triggered(false, true);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Left_Completed(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	Is_Moving_Left = value.Get<bool>();

	Change_State_If_Moving();
	Move_Button_Completed(true, Is_Moving_Right, Target_Angle_Of_Body_Lean_Left_Right);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Right_Started(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	Is_Moving_Right = value.Get<bool>();

	Change_State_If_Moving();
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Right_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	Move_For_Axis_Triggered(false, false);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Right_Completed(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	Is_Moving_Right = value.Get<bool>();

	Change_State_If_Moving();
	Move_Button_Completed(false, Is_Moving_Left, Target_Angle_Of_Body_Lean_Left_Right);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Up_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	FVector direction = GetActorUpVector() * Movement_Speed * GetWorld()->DeltaTimeSeconds * 0.5f;
	
	AddMovementInput(direction);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Down_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)
	FVector direction = GetActorUpVector() * Movement_Speed * GetWorld()->DeltaTimeSeconds * 0.5f * -1;
	
	AddMovementInput(direction);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Zoom_Increase(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Target_Camera_Distance = FMath::Clamp(Target_Camera_Distance + 100.0f, 600.0f, 800.0f);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Zoom_Decrease(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Target_Camera_Distance = FMath::Clamp(Target_Camera_Distance - 100.0f, 600.0f, 800.0f);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Turn_On_Off_Fleshlight(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Is_Flashlight_Turn_On = !Is_Flashlight_Turn_On;

	Flashlight_Component->SetVisibility(Is_Flashlight_Turn_On);

	if (GetLocalRole() < ROLE_Authority)
		Server_Turn_On_Off_Fleshlight(Is_Flashlight_Turn_On);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Interact(const FInputActionValue& value)
{
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::SetupPlayerInputComponent(UInputComponent* player_input_component)
{
	Super::SetupPlayerInputComponent(player_input_component);

	if (UEnhancedInputComponent* enhanced_input_component = CastChecked<UEnhancedInputComponent>(player_input_component))
	{
		enhanced_input_component->BindAction(Move_Forward_Action, ETriggerEvent::Started, this, &ThisClass::Move_Forward_Started);
		enhanced_input_component->BindAction(Move_Forward_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Forward_Triggered);
		enhanced_input_component->BindAction(Move_Forward_Action, ETriggerEvent::Completed, this, &ThisClass::Move_Forward_Completed);

		enhanced_input_component->BindAction(Move_Backward_Action, ETriggerEvent::Started, this, &ThisClass::Move_Backward_Started);
		enhanced_input_component->BindAction(Move_Backward_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Backward_Triggered);
		enhanced_input_component->BindAction(Move_Backward_Action, ETriggerEvent::Completed, this, &ThisClass::Move_Backward_Completed);

		enhanced_input_component->BindAction(Move_Left_Action, ETriggerEvent::Started, this, &ThisClass::Move_Left_Started);
		enhanced_input_component->BindAction(Move_Left_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Left_Triggered);
		enhanced_input_component->BindAction(Move_Left_Action, ETriggerEvent::Completed, this, &ThisClass::Move_Left_Completed);

		enhanced_input_component->BindAction(Move_Right_Action, ETriggerEvent::Started, this, &ThisClass::Move_Right_Started);
		enhanced_input_component->BindAction(Move_Right_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Right_Triggered);
		enhanced_input_component->BindAction(Move_Right_Action, ETriggerEvent::Completed, this, &ThisClass::Move_Right_Completed);

		enhanced_input_component->BindAction(Move_Up_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Up_Triggered);
		enhanced_input_component->BindAction(Move_Down_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Down_Triggered);


		enhanced_input_component->BindAction(Zoom_Decrease_Action, ETriggerEvent::Started, this, &ThisClass::Zoom_Decrease);
		enhanced_input_component->BindAction(Zoom_Increase_Action, ETriggerEvent::Started, this, &ThisClass::Zoom_Increase);

		enhanced_input_component->BindAction(Turn_On_Off_Fleshlight_Action, ETriggerEvent::Started, this, &ThisClass::Turn_On_Off_Fleshlight);

		enhanced_input_component->BindAction(Interact_Action, ETriggerEvent::Started, this, &ThisClass::Interact);
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Turn_Camera()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		float delta_mouse_x;
		float delta_mouse_y;
		PlayerController->GetInputMouseDelta(delta_mouse_x, delta_mouse_y);

		if (delta_mouse_x != 0.0f || delta_mouse_y != 0.0f)
		{
			FRotator CurrentRotation = Camera_Spring_Arm->GetRelativeRotation();

			float new_pitch = CurrentRotation.Pitch + delta_mouse_y;

			new_pitch<0.0f?new_pitch+=360.0f:new_pitch=new_pitch;

			if ((new_pitch >= 0.f && new_pitch <= 30.f) ||
				(new_pitch >= 310.f && new_pitch <= 360.f))
			{
				new_pitch = new_pitch;
			}
			else
			{
				if(new_pitch > 300.0f)
					new_pitch = 310.0f;
				else
					new_pitch = 30.0f;
			}

			FRotator new_rotation(new_pitch, CurrentRotation.Yaw + delta_mouse_x, 0.0f);

			Camera_Spring_Arm->SetRelativeRotation(new_rotation);

			if (GetLocalRole() < ROLE_Authority)
				Server_Turn_Camera(new_rotation);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_For_Axis_Triggered(bool is_forward_backward, bool is_negative_axis)
{
	if (is_forward_backward)
	{
		if (!is_negative_axis)
		{
			if (Is_Moving_Backward)
				Target_Angle_Of_Body_Lean_Forward_Backward = 0;
			else
				Target_Angle_Of_Body_Lean_Forward_Backward = Body_Rotation_Angle;
		}
		else
		{
			if (Is_Moving_Forward)
				Target_Angle_Of_Body_Lean_Forward_Backward = 0;
			else
				Target_Angle_Of_Body_Lean_Forward_Backward = -Body_Rotation_Angle;
		}
	}
	else
	{
		if (!is_negative_axis)
		{
			if (Is_Moving_Left)
				Target_Angle_Of_Body_Lean_Left_Right = 0;
			else
				Target_Angle_Of_Body_Lean_Left_Right = Body_Rotation_Angle;
		}
		else
		{
			if (Is_Moving_Right)
				Target_Angle_Of_Body_Lean_Left_Right = 0;
			else
				Target_Angle_Of_Body_Lean_Left_Right = -Body_Rotation_Angle;
		}
	}

	FVector direction =
		(is_forward_backward ? Camera_Spring_Arm->GetForwardVector() : Camera_Spring_Arm->GetRightVector())
		* Movement_Speed
		* GetWorld()->DeltaTimeSeconds
		* (is_negative_axis ? -1 : 1);
	direction.Z = 0.0f;

	if (GetLocalRole() < ROLE_Authority)
	{
		AddMovementInput(direction);
		Server_Move_For_Axis_Triggered(Target_Angle_Of_Body_Lean_Forward_Backward, Target_Angle_Of_Body_Lean_Left_Right);
	}
	else
	{
		AddMovementInput(direction);
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
void ARobot_Fly::Update_Actor_Position_MPC_Value()
{
	if (!MPC_Robot_Fly) return;

	UMaterialParameterCollectionInstance* MPC_Instance = GetWorld()->GetParameterCollectionInstance(MPC_Robot_Fly);
	if (MPC_Instance)
	{
		FVector Actor_Pos = GetActorLocation();
		MPC_Instance->SetVectorParameterValue(FName("Player_Pos"), FLinearColor(Actor_Pos) );
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

	if (GetLocalRole() < ROLE_Authority)
		Server_Change_State_If_Moving(Is_Moving);
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
void ARobot_Fly::Server_Move_For_Axis_Triggered_Implementation(const float& target_angle_of_body_lean_forward_backward, const float& target_angle_of_body_lean_left_right)
{
	Target_Angle_Of_Body_Lean_Forward_Backward = target_angle_of_body_lean_forward_backward;
	Target_Angle_Of_Body_Lean_Left_Right = target_angle_of_body_lean_left_right;
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Fly::Server_Move_For_Axis_Triggered_Validate(const float& target_angle_of_body_lean_forward_backward, const float& target_angle_of_body_lean_left_right)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Server_Change_State_If_Moving_Implementation(bool is_moving)
{
	Is_Moving = is_moving;
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Fly::Server_Change_State_If_Moving_Validate(bool is_moving)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Server_Turn_Camera_Implementation(FRotator new_rotation)
{
	Camera_Spring_Arm->SetRelativeRotation(new_rotation);
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Fly::Server_Turn_Camera_Validate(FRotator new_rotation)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Server_Turn_On_Off_Fleshlight_Implementation(bool is_flashlight_turn_on)
{
	Is_Flashlight_Turn_On = is_flashlight_turn_on;
	Flashlight_Component->SetVisibility(Is_Flashlight_Turn_On);
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Fly::Server_Turn_On_Off_Fleshlight_Validate(bool is_flashlight_turn_on)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
