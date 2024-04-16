#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "BasePlayer.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS()
class COINCOLLECTOR_API ABasePlayer : public APawn
{
	GENERATED_BODY()

public:
	ABasePlayer();

	virtual void SetupPlayerInputComponent(class UInputComponent* player_input_component);

	void Move_Up(float value);
	void Move_Right(float value);

	int Counter_Collected_Coin;

	UFUNCTION(BlueprintCallable) bool check_Jump();
	UFUNCTION(BlueprintImplementableEvent) void Jump();
	UFUNCTION(BlueprintNativeEvent) void OnOverlap(AActor* overlapped_actor, AActor* other_actor);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USpringArmComponent* Spring_Arm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Movement_Force;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Jump_Impulse;

};
//------------------------------------------------------------------------------------------------------------
