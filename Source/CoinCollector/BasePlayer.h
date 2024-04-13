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

	virtual void Tick(float DeltaTime);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	void MoveUp(float Value);
	void MoveRight(float Value);

	bool bCanJump;

	UFUNCTION(BlueprintCallable) bool checkJump();
	UFUNCTION(BlueprintImplementableEvent) void Jump();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MovementForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float JumpImpulse;

protected:
	virtual void BeginPlay() override;

};
//------------------------------------------------------------------------------------------------------------
