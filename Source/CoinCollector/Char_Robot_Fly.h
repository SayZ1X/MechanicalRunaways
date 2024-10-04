#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SceneComponent.h"
#include "Char_Robot_Fly.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract)
class COINCOLLECTOR_API AChar_Robot_Fly : public APawn
{
	GENERATED_BODY()

public:
	AChar_Robot_Fly();

	virtual void BeginPlay() override;
	virtual void Tick(float delta_seconds) override;

	//-- Input Functions --
	void Move_Forward_Triggered(bool is_button_pressed);
	void Move_Forward_Completed(bool is_button_pressed);
	void Move_Backward_Triggered(bool is_button_pressed);
	void Move_Backward_Completed(bool is_button_pressed);
	void Move_Left_Triggered(bool is_button_pressed);
	void Move_Left_Completed(bool is_button_pressed);
	void Move_Right_Triggered(bool is_button_pressed);
	void Move_Right_Completed(bool is_button_pressed);
	void Move_Up_Triggered(bool is_button_pressed);
	void Move_Down_Triggered(bool is_button_pressed);

	void Zoom_Increase();
	void Zoom_Decrease();
	void Turn_On_Off_Fleshlight();
	//-- Input Functions --

protected:
	void Turn_Camera();
	void Move_For_Axis_Triggered(bool is_forward_backward, bool is_negative_axis);
	void Move_Button_Completed(bool negative_axis_completed, bool opposite_key_triggered, float& changed_axis);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USceneComponent* Scene_Component;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USkeletalMeshComponent* Robot_Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UCapsuleComponent* Capsule_Component;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USpringArmComponent* Camera_Spring_Arm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USpotLightComponent* Flashlight_Component;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UFloatingPawnMovement* Movement_Component;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool Is_Flashlight_Turn_On;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "250.0", ClampMax = "850.0")) float Camera_Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "250.0", ClampMax = "850.0")) float Target_Camera_Distance;

	UPROPERTY(BlueprintReadOnly) float Body_Rotation_Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Body_Lean_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "15.0")) float Angle_Of_Body_Lean;
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "-15.0", ClampMax = "15.0")) float Target_Angle_Of_Body_Lean_Forward_Backward;
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "-15.0", ClampMax = "15.0")) float Interp_Angle_Of_Body_Lean_Forward_Backward;
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "-15.0", ClampMax = "15.0")) float Target_Angle_Of_Body_Lean_Left_Right;
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "-15.0", ClampMax = "15.0")) float Interp_Angle_Of_Body_Lean_Left_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool Is_Moving_Forward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool Is_Moving_Backward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool Is_Moving_Left;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool Is_Moving_Right;

};
//------------------------------------------------------------------------------------------------------------
