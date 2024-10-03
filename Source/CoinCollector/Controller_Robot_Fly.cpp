#include "Controller_Robot_Fly.h"
#include "Char_Robot_Fly.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Engine/Engine.h"

// AController_Robot_Fly
//------------------------------------------------------------------------------------------------------------
AController_Robot_Fly::AController_Robot_Fly()
{
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Fly::BeginPlay()
{
   Super::BeginPlay();

   if (APlayerController* PlayerController = Cast<APlayerController>(this))
   {
      if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
      {
         Subsystem->AddMappingContext(Input_Mapping, 0);
      }
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Fly::SetupInputComponent()
{
   Super::SetupInputComponent();

   if (UEnhancedInputComponent* enhanced_input_component = CastChecked<UEnhancedInputComponent>(InputComponent))
   {
      enhanced_input_component->BindAction(Move_Forward_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Forward_Triggered);
      //enhanced_input_component->BindAction(Move_Forward_Action, ETriggerEvent::Completed, this, &ThisClass::Move_Forward_Completed);

      enhanced_input_component->BindAction(Move_Backward_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Backward_Triggered);
      //enhanced_input_component->BindAction(Move_Backward_Action, ETriggerEvent::Completed, this, &ThisClass::Move_Backward_Completed);

      enhanced_input_component->BindAction(Move_Left_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Left_Triggered);
      //enhanced_input_component->BindAction(Move_Left_Action, ETriggerEvent::Completed, this, &ThisClass::Move_Left_Completed);

      enhanced_input_component->BindAction(Move_Right_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Right_Triggered);
      //enhanced_input_component->BindAction(Move_Right_Action, ETriggerEvent::Completed, this, &ThisClass::Move_Right_Completed);

      enhanced_input_component->BindAction(Move_Up_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Up_Triggered);
      enhanced_input_component->BindAction(Move_Down_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Down_Triggered);


      enhanced_input_component->BindAction(Zoom_Decrease_Action, ETriggerEvent::Started, this, &ThisClass::Zoom_Decrease);
      enhanced_input_component->BindAction(Zoom_Increase_Action, ETriggerEvent::Started, this, &ThisClass::Zoom_Increase);

      enhanced_input_component->BindAction(Turn_On_Off_Fleshlight_Action, ETriggerEvent::Started, this, &ThisClass::Turn_On_Off_Fleshlight);
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Fly::Move_Forward_Triggered(const FInputActionValue& value)
{
   if (AChar_Robot_Fly* player_pawn = Cast<AChar_Robot_Fly>(GetPawn()))
   {
      player_pawn->Move_Forward_Triggered(value.Get<bool>());
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Fly::Move_Backward_Triggered(const FInputActionValue& value)
{
   if (AChar_Robot_Fly* player_pawn = Cast<AChar_Robot_Fly>(GetPawn()))
   {
      player_pawn->Move_Backward_Triggered(value.Get<bool>());
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Fly::Move_Left_Triggered(const FInputActionValue& value)
{
   if (AChar_Robot_Fly* player_pawn = Cast<AChar_Robot_Fly>(GetPawn()))
   {
      player_pawn->Move_Left_Triggered(value.Get<bool>());
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Fly::Move_Right_Triggered(const FInputActionValue& value)
{
   if (AChar_Robot_Fly* player_pawn = Cast<AChar_Robot_Fly>(GetPawn()))
   {
      player_pawn->Move_Right_Triggered(value.Get<bool>());
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Fly::Move_Up_Triggered(const FInputActionValue& value)
{
   if (AChar_Robot_Fly* player_pawn = Cast<AChar_Robot_Fly>(GetPawn()))
   {
      player_pawn->Move_Up_Triggered(value.Get<bool>());
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Fly::Move_Down_Triggered(const FInputActionValue& value)
{
   if (AChar_Robot_Fly* player_pawn = Cast<AChar_Robot_Fly>(GetPawn()))
   {
      player_pawn->Move_Down_Triggered(value.Get<bool>());
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Fly::Zoom_Increase(const FInputActionValue& value)
{
   if (AChar_Robot_Fly* player_pawn = Cast<AChar_Robot_Fly>(GetPawn()))
   {
      player_pawn->Zoom_Increase();
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Fly::Zoom_Decrease(const FInputActionValue& value)
{
   if (AChar_Robot_Fly* player_pawn = Cast<AChar_Robot_Fly>(GetPawn()))
   {
      player_pawn->Zoom_Decrease();
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Fly::Turn_On_Off_Fleshlight(const FInputActionValue& value)
{
   if (AChar_Robot_Fly* player_pawn = Cast<AChar_Robot_Fly>(GetPawn()))
   {
      player_pawn->Turn_On_Off_Fleshlight();
   }
}
//------------------------------------------------------------------------------------------------------------
