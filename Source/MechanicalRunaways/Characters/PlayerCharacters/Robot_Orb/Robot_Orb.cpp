#include "Robot_Orb.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// ABasePlayer
//------------------------------------------------------------------------------------------------------------
ARobot_Orb::ARobot_Orb()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	Body_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Body_Mesh");
	Head_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Head_Mesh");
	Head_Spring_Arm = CreateDefaultSubobject<USpringArmComponent>("Head_Spring_Arm");
	Camera_Spring_Arm = CreateDefaultSubobject<USpringArmComponent>("Camera_Spring_Arm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Head_Flashlight = CreateDefaultSubobject<USpotLightComponent>("Head_Flashlight");

	RootComponent = Body_Mesh;
	Head_Spring_Arm->SetupAttachment(Body_Mesh);
	Head_Mesh->SetupAttachment(Head_Spring_Arm);
	Head_Flashlight->SetupAttachment(Head_Mesh, TEXT("Flashlight_Socket"));
	Camera_Spring_Arm->SetupAttachment(Body_Mesh);
	Camera->SetupAttachment(Camera_Spring_Arm);

	Body_Mesh->SetSimulatePhysics(true);

	Camera_Distance = 700.0f;
	Target_Camera_Distance = Camera_Distance;
	Camera_Spring_Arm->TargetArmLength = Camera_Distance;

	Is_Flashlight_Turn_On = false;
	Head_Flashlight->SetVisibility(Is_Flashlight_Turn_On);

	Movement_Speed = 100000.0f;
	Max_Movement_Speed = 500.0f;
	Moving_Damping = 0.01f;
	Stopping_Damping = 4.0f;
	Jump_Impulse = 100000.0f;
	Head_Lean_Speed = 7.0f;
	Angle_Of_Head_Lean = 20.0f;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::BeginPlay()
{
	Super::BeginPlay();
	
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	Limit_Movement_Speed();
	Change_Camera_Distance(delta_seconds);
	Change_Body_Angle_Lean(delta_seconds);
	Turn_Camera();
	Set_Turn_Head_For_Camera();
	Update_Actor_Position_MPC_Value();
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// Call the Super
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Add properties to replicated for the derived class
	DOREPLIFETIME(ARobot_Orb, Body_Mesh);
	DOREPLIFETIME(ARobot_Orb, Camera_Spring_Arm);
	DOREPLIFETIME(ARobot_Orb, Camera);
	DOREPLIFETIME(ARobot_Orb, Head_Flashlight);
	DOREPLIFETIME(ARobot_Orb, Target_Angle_Of_Head_Lean_Forward_Backward);
	DOREPLIFETIME(ARobot_Orb, Target_Angle_Of_Head_Lean_Left_Right);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Forward_Started(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Is_Moving_Forward = value.Get<bool>();

	Body_Mesh->SetLinearDamping(Moving_Damping);

	if (GetNetMode() == NM_Client)
		Server_Change_Linear_Damping(Moving_Damping);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Forward_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Move_For_Axis_Triggered(true, false);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Forward_Completed(const FInputActionValue& value)
{
	Is_Moving_Forward = value.Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Move_Button_Completed(false, Is_Moving_Backward, true);

	if (!Check_Is_Moving())
	{
		Body_Mesh->SetLinearDamping(Stopping_Damping);

		if (GetNetMode() == NM_Client)
			Server_Change_Linear_Damping(Stopping_Damping);
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Backward_Started(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Is_Moving_Backward = value.Get<bool>();

	Body_Mesh->SetLinearDamping(Moving_Damping);

	if (GetNetMode() == NM_Client)
		Server_Change_Linear_Damping(Moving_Damping);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Backward_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Move_For_Axis_Triggered(true, true);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Backward_Completed(const FInputActionValue& value)
{
	Is_Moving_Backward = value.Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Move_Button_Completed(true, Is_Moving_Forward, true);

	if (!Check_Is_Moving())
	{
		Body_Mesh->SetLinearDamping(Stopping_Damping);

		if (GetNetMode() == NM_Client)
			Server_Change_Linear_Damping(Stopping_Damping);
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Left_Started(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Is_Moving_Left = value.Get<bool>();

	Body_Mesh->SetLinearDamping(Moving_Damping);

	if (GetNetMode() == NM_Client)
		Server_Change_Linear_Damping(Moving_Damping);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Left_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Move_For_Axis_Triggered(false, true);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Left_Completed(const FInputActionValue& value)
{
	Is_Moving_Left = value.Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Move_Button_Completed(true, Is_Moving_Right, false);

	if (!Check_Is_Moving())
	{
		Body_Mesh->SetLinearDamping(Stopping_Damping);

		if (GetNetMode() == NM_Client)
			Server_Change_Linear_Damping(Stopping_Damping);
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Right_Started(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Is_Moving_Right = value.Get<bool>();

	Body_Mesh->SetLinearDamping(Moving_Damping);

	if (GetNetMode() == NM_Client)
		Server_Change_Linear_Damping(Moving_Damping);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Right_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Move_For_Axis_Triggered(false, false);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Right_Completed(const FInputActionValue& value)
{
	Is_Moving_Right = value.Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Move_Button_Completed(false, Is_Moving_Left, false);

	if (!Check_Is_Moving())
	{
		Body_Mesh->SetLinearDamping(Stopping_Damping);

		if (GetNetMode() == NM_Client)
			Server_Change_Linear_Damping(Stopping_Damping);
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Jump(const FInputActionValue& value)
{
	if(Check_Jump() )
	{
		UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

		if (GetLocalRole() < ROLE_Authority)
		{
			Server_Jump();
			return;
		}

		Body_Mesh->SetLinearDamping(Moving_Damping);
		Body_Mesh->AddImpulse(FVector(0, 0, Jump_Impulse) );
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Zoom_Increase(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Target_Camera_Distance = FMath::Clamp(Target_Camera_Distance + 100.0f, 600.0f, 800.0f);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Zoom_Decrease(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Target_Camera_Distance = FMath::Clamp(Target_Camera_Distance - 100.0f, 600.0f, 800.0f);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Turn_On_Off_Fleshlight(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%S"), __FUNCTION__)

	Is_Flashlight_Turn_On = !Is_Flashlight_Turn_On;
	
	if (GetLocalRole() < ROLE_Authority)
		Server_Turn_On_Off_Fleshlight(Is_Flashlight_Turn_On);
	else
		Head_Flashlight->SetVisibility(Is_Flashlight_Turn_On);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Interact(const FInputActionValue& value)
{
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Turn_Camera()
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

			new_pitch < 0.0f ? new_pitch += 360.0f : new_pitch = new_pitch;

			if ((new_pitch >= 0.f && new_pitch <= 30.f) ||
				(new_pitch >= 310.f && new_pitch <= 360.f))
			{
				new_pitch = new_pitch;
			}
			else
			{
				if (new_pitch > 300.0f)
					new_pitch = 310.0f;
				else
					new_pitch = 30.0f;
			}

			FRotator new_rotation(new_pitch, CurrentRotation.Yaw + delta_mouse_x, 0.0f);
				
			if (GetLocalRole() < ROLE_Authority)
				Server_Turn_Camera(new_rotation);
			else
				Camera_Spring_Arm->SetRelativeRotation(new_rotation);
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
		if (!is_negative_axis)
		{
			if (Is_Moving_Backward)
				Target_Angle_Of_Head_Lean_Forward_Backward = 0;
			else
				Target_Angle_Of_Head_Lean_Forward_Backward = Angle_Of_Head_Lean;
		}
		else
		{
			if (Is_Moving_Forward)
				Target_Angle_Of_Head_Lean_Forward_Backward = 0;
			else
				Target_Angle_Of_Head_Lean_Forward_Backward = -Angle_Of_Head_Lean;
		}
	}
	else
	{
		if (!is_negative_axis)
		{
			if (Is_Moving_Left)
				Target_Angle_Of_Head_Lean_Left_Right = 0;
			else
				Target_Angle_Of_Head_Lean_Left_Right = Angle_Of_Head_Lean;
		}
		else
		{
			if (Is_Moving_Right)
				Target_Angle_Of_Head_Lean_Left_Right = 0;
			else
				Target_Angle_Of_Head_Lean_Left_Right = -Angle_Of_Head_Lean;
		}
	}

	FVector camera_vector = is_forward_backward?Camera->GetForwardVector():Camera->GetRightVector();
	FVector body_vector = is_forward_backward?Body_Mesh->GetForwardVector():Body_Mesh->GetRightVector();

	FVector completed_vector = FVector(camera_vector.X, camera_vector.Y, body_vector.Z);

	
	
	if (GetLocalRole() < ROLE_Authority)
	{
		completed_vector *= is_negative_axis ? -Movement_Speed : Movement_Speed;
		Server_Move_For_Axis_Triggered(completed_vector, Target_Angle_Of_Head_Lean_Forward_Backward, Target_Angle_Of_Head_Lean_Left_Right);
	}
	else
	{
		Body_Mesh->AddForce(completed_vector * (is_negative_axis ? -Movement_Speed : Movement_Speed));
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Move_Button_Completed(bool negative_axis_completed, bool opposite_key_triggered, bool is_forward_backward) //float& changed_axis)
{
	if (negative_axis_completed)
	{
		if (opposite_key_triggered)
			(is_forward_backward?Target_Angle_Of_Head_Lean_Forward_Backward:Target_Angle_Of_Head_Lean_Left_Right) = Angle_Of_Head_Lean;
		else
			(is_forward_backward?Target_Angle_Of_Head_Lean_Forward_Backward:Target_Angle_Of_Head_Lean_Left_Right) = 0;
	}
	else
	{
		if (opposite_key_triggered)
			(is_forward_backward?Target_Angle_Of_Head_Lean_Forward_Backward:Target_Angle_Of_Head_Lean_Left_Right) = -Angle_Of_Head_Lean;
		else
			(is_forward_backward?Target_Angle_Of_Head_Lean_Forward_Backward:Target_Angle_Of_Head_Lean_Left_Right) = 0;
	}

	if(GetNetMode() == NM_Client)
		Server_Move_Button_Completed(is_forward_backward, is_forward_backward?Target_Angle_Of_Head_Lean_Forward_Backward:Target_Angle_Of_Head_Lean_Left_Right);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Update_Actor_Position_MPC_Value()
{
	if (!MPC_Robot_Orb) return;

	UMaterialParameterCollectionInstance *MPC_Instance = GetWorld()->GetParameterCollectionInstance(MPC_Robot_Orb);
	if(MPC_Instance)
	{
		FVector Actor_Pos = GetActorLocation();
		MPC_Instance->SetVectorParameterValue(FName("Player_Pos"), FLinearColor(Actor_Pos) );
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Limit_Movement_Speed()
{
	if (Body_Mesh)
	{
		FVector Velocity = Body_Mesh->GetPhysicsLinearVelocity();
		float CurrentSpeed = Velocity.Size();

		if (CurrentSpeed > Max_Movement_Speed)
		{
			FVector ClampedVelocity = Velocity.GetSafeNormal() * Max_Movement_Speed;
			Body_Mesh->SetPhysicsLinearVelocity(ClampedVelocity);
		}
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
void ARobot_Orb::Change_Camera_Distance(float delta_seconds)
{
	if (Camera_Distance != Target_Camera_Distance)
	{
		Camera_Distance = FMath::FInterpTo(Camera_Distance, Target_Camera_Distance, delta_seconds, 2.0f);
		Camera_Spring_Arm->TargetArmLength = Camera_Distance;
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Change_Body_Angle_Lean(float delta_seconds)
{
	Interp_Angle_Of_Head_Lean_Forward_Backward = FMath::FInterpTo(Interp_Angle_Of_Head_Lean_Forward_Backward, Target_Angle_Of_Head_Lean_Forward_Backward, delta_seconds, Head_Lean_Speed);
	Interp_Angle_Of_Head_Lean_Left_Right = FMath::FInterpTo(Interp_Angle_Of_Head_Lean_Left_Right, Target_Angle_Of_Head_Lean_Left_Right, delta_seconds, Head_Lean_Speed);
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Orb::Check_Is_Moving()
{
	if (Is_Moving_Forward || Is_Moving_Backward || Is_Moving_Left || Is_Moving_Right)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Server_Move_For_Axis_Triggered_Implementation(const FVector& movement_vector, const float& target_angle_of_head_lean_forward_backward, const float& target_angle_of_head_lean_left_right)
{
	Target_Angle_Of_Head_Lean_Forward_Backward = target_angle_of_head_lean_forward_backward;
	Target_Angle_Of_Head_Lean_Left_Right = target_angle_of_head_lean_left_right;

	Body_Mesh->AddForce(movement_vector);
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Orb::Server_Move_For_Axis_Triggered_Validate(const FVector& movement_vector, const float& target_angle_of_head_lean_forward_backward, const float& target_angle_of_head_lean_left_right)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Server_Move_Button_Completed_Implementation(bool is_forward_backward, const float& target_angle_lean)
{
	if(is_forward_backward)
	{
		Target_Angle_Of_Head_Lean_Forward_Backward = target_angle_lean;
	}
	else
	{
		Target_Angle_Of_Head_Lean_Left_Right = target_angle_lean;
	}
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Orb::Server_Move_Button_Completed_Validate(bool is_forward_backward, const float& target_angle_lean)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Server_Change_Linear_Damping_Implementation(const float& damping)
{
	Body_Mesh->SetLinearDamping(damping);
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Orb::Server_Change_Linear_Damping_Validate(const float& damping)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Server_Jump_Implementation()
{
	Body_Mesh->SetLinearDamping(Moving_Damping);
	Body_Mesh->AddImpulse(FVector(0, 0, Jump_Impulse));
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Orb::Server_Jump_Validate()
{
	return Check_Jump();
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Server_Turn_Camera_Implementation(FRotator new_rotation)
{
	Camera_Spring_Arm->SetRelativeRotation(new_rotation);
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Orb::Server_Turn_Camera_Validate(FRotator new_rotation)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::Server_Turn_On_Off_Fleshlight_Implementation(bool is_flashlight_turn_on)
{
	Is_Flashlight_Turn_On = is_flashlight_turn_on;

	Head_Flashlight->SetVisibility(Is_Flashlight_Turn_On);
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Orb::Server_Turn_On_Off_Fleshlight_Validate(bool is_flashlight_turn_on)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Orb::SetupPlayerInputComponent(UInputComponent* player_input_component)
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

		enhanced_input_component->BindAction(Jump_Action, ETriggerEvent::Started, this, &ThisClass::Jump);

		enhanced_input_component->BindAction(Zoom_Decrease_Action, ETriggerEvent::Started, this, &ThisClass::Zoom_Decrease);
		enhanced_input_component->BindAction(Zoom_Increase_Action, ETriggerEvent::Started, this, &ThisClass::Zoom_Increase);

		enhanced_input_component->BindAction(Turn_On_Off_Fleshlight_Action, ETriggerEvent::Started, this, &ThisClass::Turn_On_Off_Fleshlight);
		enhanced_input_component->BindAction(Interact_Action, ETriggerEvent::Started, this, &ThisClass::Interact);
	}
}
//------------------------------------------------------------------------------------------------------------
