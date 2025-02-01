#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerCharacters/Player_Character/Player_Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Robot_Orb.generated.h"

class UInputMappingContext;
class UInputAction;

//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract)
class MECHANICALRUNAWAYS_API ARobot_Orb : public APlayer_Character
{
	GENERATED_BODY()

public:
	ARobot_Orb();

	virtual void BeginPlay() override;
	virtual void Tick(float delta_seconds) override;

	//-- Input Functions --
	void Move_Forward_Triggered(const FInputActionValue& value);
	void Move_Forward_Completed(const FInputActionValue& value);
	void Move_Backward_Triggered(const FInputActionValue& value);
	void Move_Backward_Completed(const FInputActionValue& value);
	void Move_Left_Triggered(const FInputActionValue& value);
	void Move_Left_Completed(const FInputActionValue& value);
	void Move_Right_Triggered(const FInputActionValue& value);
	void Move_Right_Completed(const FInputActionValue& value);
	void Jump(const FInputActionValue& value);
	void Zoom_Increase(const FInputActionValue& value);
	void Zoom_Decrease(const FInputActionValue& value);
	void Turn_On_Off_Fleshlight(const FInputActionValue& value);
	void Interact(const FInputActionValue& value);
	//-- Input Functions --

protected:
	void Turn_Camera();
	void Set_Turn_Head_For_Camera();
	void Move_For_Axis_Triggered(bool is_forward_backward, bool is_negative_axis);
	void Move_Button_Completed(bool negative_axis_completed, bool opposite_key_triggered, float& changed_axis);

	UFUNCTION(BlueprintCallable) bool Check_Jump();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USpringArmComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UStaticMeshComponent* Body_Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USpringArmComponent* Head_Spring_Arm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USkeletalMeshComponent* Head_Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USpringArmComponent* Camera_Spring_Arm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USpotLightComponent* Head_Flashlight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool Is_Flashlight_Turn_On;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Movement_Force;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Jump_Impulse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "550.0", ClampMax = "1350.0")) float Camera_Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "550.0", ClampMax = "1350.0")) float Target_Camera_Distance;

	UPROPERTY(BlueprintReadOnly) float Head_Rotation_Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Head_Lean_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "90.0") ) float Angle_Of_Head_Lean;
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "-90.0", ClampMax = "90.0")) float Target_Angle_Of_Head_Lean_Forward_Backward;
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "-90.0", ClampMax = "90.0")) float Interp_Angle_Of_Head_Lean_Forward_Backward;
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "-90.0", ClampMax = "90.0")) float Target_Angle_Of_Head_Lean_Left_Right;
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "-90.0", ClampMax = "90.0")) float Interp_Angle_Of_Head_Lean_Left_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool Is_Moving_Forward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool Is_Moving_Backward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool Is_Moving_Left;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool Is_Moving_Right;

	virtual void SetupPlayerInputComponent(UInputComponent* player_input_component) override;

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
	UInputAction* Jump_Action;

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
