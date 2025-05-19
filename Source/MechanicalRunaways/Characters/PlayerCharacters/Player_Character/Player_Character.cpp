#include "Characters/PlayerCharacters/Player_Character/Player_Character.h"
#include "Kismet/KismetSystemLibrary.h"




// APlayer_Character
//------------------------------------------------------------------------------------------------------------
APlayer_Character::APlayer_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Character::BeginPlay()
{
	Super::BeginPlay();

}
//------------------------------------------------------------------------------------------------------------
void APlayer_Character::Tick(float delta_time)
{
	Super::Tick(delta_time);

}
//------------------------------------------------------------------------------------------------------------
void APlayer_Character::Server_Open_Close_Door_Implementation(AActor* door_ref)
{
	Multicast_Open_Close_Door_Implementation(door_ref);
}
//------------------------------------------------------------------------------------------------------------
bool APlayer_Character::Server_Open_Close_Door_Validate(AActor* door_ref)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Character::Multicast_Open_Close_Door_Implementation(AActor* door_ref)
{
	if (door_ref->GetClass()->GetName() == "BP_Door_With_Pass_C" ||
		door_ref->GetClass()->GetName() == "BP_Huge_Door_C")
	{
		FName FunctionName = "Open_Close_Door";
		UFunction* Function = door_ref->FindFunction(FunctionName);

		if (Function)
		{
			door_ref->ProcessEvent(Function, nullptr);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
bool APlayer_Character::Multicast_Open_Close_Door_Validate(AActor* door_ref)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Character::Server_Activate_Door_Implementation(AActor* door_ref)
{
	Multicast_Activate_Door(door_ref);
}
//------------------------------------------------------------------------------------------------------------
bool APlayer_Character::Server_Activate_Door_Validate(AActor* door_ref)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Character::Multicast_Activate_Door_Implementation(AActor* door_ref)
{
	if (door_ref->GetClass()->GetName() == "BP_Huge_Door_C")
	{
		FName FunctionName = "Activate_Door";
		UFunction* Function = door_ref->FindFunction(FunctionName);

		if (Function)
		{
			door_ref->ProcessEvent(Function, nullptr);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
bool APlayer_Character::Multicast_Activate_Door_Validate(AActor* door_ref)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------
