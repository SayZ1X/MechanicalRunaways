#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
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

	virtual void BeginPlay();

	void Move_Up(float value);
	void Move_Right(float value);
	void Jump();
	void Collect_Coin(ABase_Coin* coin);

	int Counter_Collected_Coin;

	UPROPERTY(BlueprintAssignable) FCounter_Collected_Coin_Changed Counter_Collected_Coin_Changed;

protected:
	UFUNCTION(BlueprintCallable) bool Check_Jump();
	UFUNCTION(BlueprintNativeEvent) void OnOverlap(AActor* overlapped_actor, AActor* other_actor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USpringArmComponent* Spring_Arm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Movement_Force;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Jump_Impulse;

};
//------------------------------------------------------------------------------------------------------------
