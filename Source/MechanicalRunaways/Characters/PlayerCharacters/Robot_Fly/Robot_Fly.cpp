#include "Robot_Fly.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Materials/MaterialParameterCollectionInstance.h"

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
	SetRootComponent(Capsule_Component);
	Robot_Mesh->SetupAttachment(Capsule_Component);
	Camera_Spring_Arm->SetupAttachment(Robot_Mesh);
	Camera->SetupAttachment(Camera_Spring_Arm);
	Flashlight_Component->SetupAttachment(Robot_Mesh, TEXT("Flashlight_Socket"));

	Is_Flashlight_Turn_On = false;
	Flashlight_Component->SetVisibility(Is_Flashlight_Turn_On);

	Movement_Speed = 600.0f;

	Camera_Distance = 550.0f;
	Target_Camera_Distance = Camera_Distance;
	Camera_Spring_Arm->TargetArmLength = Camera_Distance;

	Body_Lean_Speed = 4.0f;
	Body_Rotation_Angle = 15.0f;

	Blades_Current_Rotation = 0.0f;
	Blades_Rotation_Speed = 1800.0f;

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	Capsule_Component->SetIsReplicated(true);
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
void ARobot_Fly::Move_Forward_Started(const FInputActionValue& value)
{
	Is_Moving_Forward = value.Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Change_State_If_Moving();
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Forward_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Move_For_Axis_Triggered(true, false);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Forward_Completed(const FInputActionValue& value)
{
	Is_Moving_Forward = value.Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Change_State_If_Moving();
	Move_Button_Completed(false, Is_Moving_Backward, Target_Angle_Of_Body_Lean_Forward_Backward);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Backward_Started(const FInputActionValue& value)
{
	Is_Moving_Backward = value.Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Change_State_If_Moving();
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Backward_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Move_For_Axis_Triggered(true, true);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Backward_Completed(const FInputActionValue& value)
{
	Is_Moving_Backward = value.Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Change_State_If_Moving();
	Move_Button_Completed(true, Is_Moving_Forward, Target_Angle_Of_Body_Lean_Forward_Backward);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Left_Started(const FInputActionValue& value)
{
	Is_Moving_Left = value.Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Change_State_If_Moving();
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Left_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Move_For_Axis_Triggered(false, true);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Left_Completed(const FInputActionValue& value)
{
	Is_Moving_Left = value.Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Change_State_If_Moving();
	Move_Button_Completed(true, Is_Moving_Right, Target_Angle_Of_Body_Lean_Left_Right);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Right_Started(const FInputActionValue& value)
{
	Is_Moving_Right = value.Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Change_State_If_Moving();
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Right_Triggered(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Move_For_Axis_Triggered(false, false);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Right_Completed(const FInputActionValue& value)
{
	Is_Moving_Right = value.Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Change_State_If_Moving();
	Move_Button_Completed(false, Is_Moving_Left, Target_Angle_Of_Body_Lean_Left_Right);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Up_Triggered(const FInputActionValue& value)
{
	FVector direction = GetActorUpVector() * Movement_Speed * GetWorld()->DeltaTimeSeconds * 0.5f;
	
	if(Check_Can_Move(direction) )
	{
		RootComponent->AddRelativeLocation(direction);
		Server_Move_Up(direction);
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Move_Down_Triggered(const FInputActionValue& value)
{
	FVector direction = GetActorUpVector() * Movement_Speed * GetWorld()->DeltaTimeSeconds * 0.5f * -1;
	
	if (Check_Can_Move(direction))
	{
		RootComponent->AddRelativeLocation(direction);
		Server_Move_Down(direction);
	}
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Zoom_Increase(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Target_Camera_Distance = FMath::Clamp(Target_Camera_Distance + 100.0f, 250.0f, 850.0f);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Zoom_Decrease(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Target_Camera_Distance = FMath::Clamp(Target_Camera_Distance - 100.0f, 250.0f, 850.0f);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Turn_On_Off_Fleshlight(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), __FUNCTION__)

	Is_Flashlight_Turn_On = !Is_Flashlight_Turn_On;

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

			if (GetNetMode() == NM_Client)
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

	if(Check_Can_Move(direction) )
	{
		RootComponent->AddRelativeLocation(direction);

		if (GetNetMode() == NM_Client)
		{
			Server_Move_For_Axis_Triggered(direction, Target_Angle_Of_Body_Lean_Forward_Backward, Target_Angle_Of_Body_Lean_Left_Right);
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
bool ARobot_Fly::Check_Can_Move(const FVector& direction)
{
	FVector world_direction = GetActorRotation().RotateVector(direction);
	FVector start = /*RootComponent->GetRelativeLocation()*/GetActorLocation();
	FVector end = start + world_direction * 100.0f;

	FHitResult hit_result;

	FCollisionQueryParams trace_params(FName(TEXT("TraceForward")), true, this);
	trace_params.bTraceComplex = true; // Трассировка сложной геометрии
	trace_params.AddIgnoredActor(this); // Игнорировать себя

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		hit_result,              // Результат столкновения
		start,                  // Начало перемещения
		end,                    // Конец перемещения
		ECC_Visibility,         // Канал столкновений
		trace_params             // Параметры трассировки
	);

	return bHit?false:true;
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
void ARobot_Fly::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// Call the Super
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Add properties to replicated for the derived class
	DOREPLIFETIME(ARobot_Fly, Robot_Mesh);
	DOREPLIFETIME(ARobot_Fly, Capsule_Component);
	DOREPLIFETIME(ARobot_Fly, Camera_Spring_Arm);
	DOREPLIFETIME(ARobot_Fly, Flashlight_Component);
	DOREPLIFETIME(ARobot_Fly, Body_Rotation_Angle);
	DOREPLIFETIME(ARobot_Fly, Target_Angle_Of_Body_Lean_Forward_Backward);
	DOREPLIFETIME(ARobot_Fly, Target_Angle_Of_Body_Lean_Left_Right);

	DOREPLIFETIME(ARobot_Fly, Is_Moving);
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Server_Move_For_Axis_Triggered_Implementation(const FVector& direction, const float& target_angle_of_body_lean_forward_backward, const float& target_angle_of_body_lean_left_right)
{
	Target_Angle_Of_Body_Lean_Forward_Backward = target_angle_of_body_lean_forward_backward;
	Target_Angle_Of_Body_Lean_Left_Right = target_angle_of_body_lean_left_right;

	RootComponent->AddRelativeLocation(direction);
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Fly::Server_Move_For_Axis_Triggered_Validate(const FVector& direction, const float& target_angle_of_body_lean_forward_backward, const float& target_angle_of_body_lean_left_right)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Server_Move_Up_Implementation(const FVector& direction)
{
	RootComponent->AddRelativeLocation(direction);
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Fly::Server_Move_Up_Validate(const FVector& direction)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
void ARobot_Fly::Server_Move_Down_Implementation(const FVector& direction)
{
	RootComponent->AddRelativeLocation(direction);
}
//------------------------------------------------------------------------------------------------------------
bool ARobot_Fly::Server_Move_Down_Validate(const FVector& direction)
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
