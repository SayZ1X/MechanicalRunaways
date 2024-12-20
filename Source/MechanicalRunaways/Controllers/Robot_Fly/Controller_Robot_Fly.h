#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Controller_Robot_Fly.generated.h"

class ARobot_Fly;
class UInputMappingContext;
class UInputAction;

//------------------------------------------------------------------------------------------------------------
UCLASS()
class MECHANICALRUNAWAYS_API AController_Robot_Fly : public APlayerController
{
	GENERATED_BODY()
	
public:
	AController_Robot_Fly();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	//-- Input Functions --
	void Move_Forward_Triggered(const FInputActionValue& value);
	void Move_Forward_Completed(const FInputActionValue& value);
	void Move_Backward_Triggered(const FInputActionValue& value);
	void Move_Backward_Completed(const FInputActionValue& value);
	void Move_Left_Triggered(const FInputActionValue& value);
	void Move_Left_Completed(const FInputActionValue& value);
	void Move_Right_Triggered(const FInputActionValue& value);
	void Move_Right_Completed(const FInputActionValue& value);
	void Move_Up_Triggered(const FInputActionValue& value);
	void Move_Down_Triggered(const FInputActionValue& value);

	void Zoom_Increase(const FInputActionValue& value);
	void Zoom_Decrease(const FInputActionValue& value);
	void Turn_On_Off_Fleshlight(const FInputActionValue& value);

	//-- Input Actions -- 
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
	UInputAction* Zoom_Increase_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* Zoom_Decrease_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* Turn_On_Off_Fleshlight_Action;

};
//------------------------------------------------------------------------------------------------------------
