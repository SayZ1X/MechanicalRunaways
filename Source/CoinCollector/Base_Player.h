#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Base_Player.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCounter_Collected_Coin_Changed, int, New_Count_Collected_Coin);
class ABase_Coin;

//------------------------------------------------------------------------------------------------------------
UCLASS()
class COINCOLLECTOR_API ABase_Player : public APawn
{
	GENERATED_BODY()

public:
	ABase_Player();

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
	void Jump();
	void Zoom_Increase();
	void Zoom_Decrease();
	void Turn_On_Off_Fleshlight();
	//-- Input Functions --

	void Collect_Coin(ABase_Coin* coin);

	int Counter_Collected_Coin;

	UPROPERTY(BlueprintAssignable) FCounter_Collected_Coin_Changed Counter_Collected_Coin_Changed;

protected:
	void Turn_Camera();
	void Set_Turn_Head_For_Camera();
	void Move_For_Axis_Triggered(bool is_forward_backward, bool is_negative_axis);
	void Move_Button_Completed(bool negative_axis_completed, bool opposite_key_triggered, float& changed_axis);

	UFUNCTION(BlueprintCallable) bool Check_Jump();
	UFUNCTION(BlueprintNativeEvent) void OnOverlap(AActor* overlapped_actor, AActor* other_actor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USceneComponent* Scene_Component;
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

};
//------------------------------------------------------------------------------------------------------------
