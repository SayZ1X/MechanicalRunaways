#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/SpotLightComponent.h"
#include "Components/SKeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/MaterialParameterCollection.h"
#include "Net/UnrealNetwork.h"
#include "Robot_Fly.generated.h"

class UInputMappingContext;
class UInputAction;

//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract)
class MECHANICALRUNAWAYS_API ARobot_Fly : public ACharacter
{
	GENERATED_BODY()

public:
	ARobot_Fly();

	virtual void BeginPlay() override;
	virtual void Tick(float delta_seconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//-- Input Functions --
	void Move_Forward_Started(const FInputActionValue& value);
	void Move_Forward_Triggered(const FInputActionValue& value);
	void Move_Forward_Completed(const FInputActionValue& value);

	void Move_Backward_Started(const FInputActionValue& value);
	void Move_Backward_Triggered(const FInputActionValue& value);
	void Move_Backward_Completed(const FInputActionValue& value);

	void Move_Left_Started(const FInputActionValue& value);
	void Move_Left_Triggered(const FInputActionValue& value);
	void Move_Left_Completed(const FInputActionValue& value);

	void Move_Right_Started(const FInputActionValue& value);
	void Move_Right_Triggered(const FInputActionValue& value);
	void Move_Right_Completed(const FInputActionValue& value);

	void Move_Up_Triggered(const FInputActionValue& value);
	void Move_Down_Triggered(const FInputActionValue& value);

	void Acceleration_Started(const FInputActionValue& value);
	void Acceleration_Completed(const FInputActionValue& value);

	void Zoom_Increase(const FInputActionValue& value);
	void Zoom_Decrease(const FInputActionValue& value);
	void Turn_On_Off_Fleshlight(const FInputActionValue& value);
	void Interact(const FInputActionValue& value);
	//-- Input Functions --

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Turn_Camera();
	void Move_For_Axis_Triggered(bool is_forward_backward, bool is_negative_axis);
	void Move_Button_Completed(bool negative_axis_completed, bool opposite_key_triggered, float& changed_axis);
	void Update_Actor_Position_MPC_Value();

	UFUNCTION(BlueprintCallable) void Change_State_If_Moving();
	UFUNCTION(BlueprintCallable) bool Get_Is_Moving();
	UFUNCTION(BlueprintCallable) void Rotate_Blades(float delta_seconds);
	UFUNCTION(BlueprintCallable) void Change_Camera_Distance(float delta_seconds);
	UFUNCTION(BlueprintCallable) void Change_Body_Angle_Lean(float delta_seconds);

	//-- Server Functions --
	UFUNCTION(BlueprintCallable, Server, WithValidation, Unreliable)
	void Server_Move_For_Axis_Triggered(const float& target_angle_of_body_lean_forward_backward, const float& target_angle_of_body_lean_left_right);
	void Server_Move_For_Axis_Triggered_Implementation(const float& target_angle_of_body_lean_forward_backward, const float& target_angle_of_body_lean_left_right);
	bool Server_Move_For_Axis_Triggered_Validate(const float& target_angle_of_body_lean_forward_backward, const float& target_angle_of_body_lean_left_right);

	UFUNCTION(BlueprintCallable, Server, WithValidation, Unreliable)
	void Server_Change_State_If_Moving(bool is_moving);
	void Server_Change_State_If_Moving_Implementation(bool is_moving);
	bool Server_Change_State_If_Moving_Validate(bool is_moving);

	UFUNCTION(BlueprintCallable, Server, WithValidation, Unreliable)
	void Server_Turn_Camera(FRotator new_rotation);
	void Server_Turn_Camera_Implementation(FRotator new_rotation);
	bool Server_Turn_Camera_Validate(FRotator new_rotation);

	UFUNCTION(BlueprintCallable, Server, WithValidation, Unreliable)
	void Server_Turn_On_Off_Fleshlight(bool is_flashlight_turn_on);
	void Server_Turn_On_Off_Fleshlight_Implementation(bool is_flashlight_turn_on);
	bool Server_Turn_On_Off_Fleshlight_Validate(bool is_flashlight_turn_on);
	//-- Server Functions --

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated) USpringArmComponent* Camera_Spring_Arm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated) USpotLightComponent* Flashlight_Component;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "MPC") UMaterialParameterCollection* MPC_Robot_Fly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool Is_Flashlight_Turn_On;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "600.0", ClampMax = "800.0")) float Camera_Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "600.0", ClampMax = "800.0")) float Target_Camera_Distance;

	UPROPERTY(BlueprintReadOnly, Replicated) float Body_Rotation_Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Body_Lean_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "15.0"), Replicated) float Angle_Of_Body_Lean;
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "-15.0", ClampMax = "15.0"), Replicated) float Target_Angle_Of_Body_Lean_Forward_Backward;
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "-15.0", ClampMax = "15.0"), Replicated) float Interp_Angle_Of_Body_Lean_Forward_Backward;
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "-15.0", ClampMax = "15.0"), Replicated) float Target_Angle_Of_Body_Lean_Left_Right;
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "-15.0", ClampMax = "15.0"), Replicated) float Interp_Angle_Of_Body_Lean_Left_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated) bool Is_Moving;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated) bool Is_Moving_Forward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated) bool Is_Moving_Backward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated) bool Is_Moving_Left;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated) bool Is_Moving_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Movement_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Blades_Current_Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Blades_Rotation_Speed; //RPM

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* Input_Mapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* Move_Forward_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* Move_Backward_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* Move_Left_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* Move_Right_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* Move_Up_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* Move_Down_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* Acceleration_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* Zoom_Increase_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* Zoom_Decrease_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* Turn_On_Off_Fleshlight_Action;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* Interact_Action;
};
//------------------------------------------------------------------------------------------------------------
