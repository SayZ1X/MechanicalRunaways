#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
#include "Player_Character.generated.h"

class UInputMappingContext;
class UInputAction;

//------------------------------------------------------------------------------------------------------------
UCLASS()
class MECHANICALRUNAWAYS_API APlayer_Character : public APawn
{
	GENERATED_BODY()

public:
	APlayer_Character();

	virtual void BeginPlay() override;
	virtual void Tick(float delta_time) override;

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
	void Server_Open_Close_Door(AActor* door_ref);
	void Server_Open_Close_Door_Implementation(AActor* door_ref);
	bool Server_Open_Close_Door_Validate(AActor* door_ref);

	UFUNCTION(BlueprintCallable, NetMulticast, WithValidation, Reliable)
	void Multicast_Open_Close_Door(AActor* door_ref);
	void Multicast_Open_Close_Door_Implementation(AActor* door_ref);
	bool Multicast_Open_Close_Door_Validate(AActor* door_ref);

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
	void Server_Activate_Door(AActor* door_ref);
	void Server_Activate_Door_Implementation(AActor* door_ref);
	bool Server_Activate_Door_Validate(AActor* door_ref);

	UFUNCTION(BlueprintCallable, NetMulticast, WithValidation, Reliable)
	void Multicast_Activate_Door(AActor* door_ref);
	void Multicast_Activate_Door_Implementation(AActor* door_ref);
	bool Multicast_Activate_Door_Validate(AActor* door_ref);
};
//------------------------------------------------------------------------------------------------------------
