#include "Controller_Robot_Orb.h"
#include "././Characters/PlayerCharacters/Robot_Orb/Robot_Orb.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// AController_Robot_Orb
//------------------------------------------------------------------------------------------------------------
AController_Robot_Orb::AController_Robot_Orb()
{
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Orb::BeginPlay()
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
void AController_Robot_Orb::SetupInputComponent()
{
	Super::SetupInputComponent();

   if (UEnhancedInputComponent* enhanced_input_component = CastChecked<UEnhancedInputComponent>(InputComponent) )
   {
      enhanced_input_component->BindAction(Move_Forward_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Forward_Triggered);
      enhanced_input_component->BindAction(Move_Forward_Action, ETriggerEvent::Completed, this, &ThisClass::Move_Forward_Completed);

      enhanced_input_component->BindAction(Move_Backward_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Backward_Triggered);
      enhanced_input_component->BindAction(Move_Backward_Action, ETriggerEvent::Completed, this, &ThisClass::Move_Backward_Completed);

      enhanced_input_component->BindAction(Move_Left_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Left_Triggered);
      enhanced_input_component->BindAction(Move_Left_Action, ETriggerEvent::Completed, this, &ThisClass::Move_Left_Completed);

      enhanced_input_component->BindAction(Move_Right_Action, ETriggerEvent::Triggered, this, &ThisClass::Move_Right_Triggered);
      enhanced_input_component->BindAction(Move_Right_Action, ETriggerEvent::Completed, this, &ThisClass::Move_Right_Completed);

      enhanced_input_component->BindAction(Jump_Action, ETriggerEvent::Started, this, &ThisClass::Jump);
   
      enhanced_input_component->BindAction(Zoom_Decrease_Action, ETriggerEvent::Started, this, &ThisClass::Zoom_Decrease);
      enhanced_input_component->BindAction(Zoom_Increase_Action, ETriggerEvent::Started, this, &ThisClass::Zoom_Increase);

      enhanced_input_component->BindAction(Turn_On_Off_Fleshlight_Action, ETriggerEvent::Started, this, &ThisClass::Turn_On_Off_Fleshlight);
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Orb::Move_Forward_Triggered(const FInputActionValue& value)
{
   if (ARobot_Orb* player_pawn = Cast<ARobot_Orb>(GetPawn() ) )
   {
      player_pawn->Move_Forward_Triggered(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Orb::Move_Forward_Completed(const FInputActionValue& value)
{
   if (ARobot_Orb* player_pawn = Cast<ARobot_Orb>(GetPawn() ) )
   {
      player_pawn->Move_Forward_Completed(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Orb::Move_Backward_Triggered(const FInputActionValue& value)
{
   if (ARobot_Orb* player_pawn = Cast<ARobot_Orb>(GetPawn() ) )
   {
      player_pawn->Move_Backward_Triggered(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Orb::Move_Backward_Completed(const FInputActionValue& value)
{
   if (ARobot_Orb* player_pawn = Cast<ARobot_Orb>(GetPawn() ) )
   {
      player_pawn->Move_Backward_Completed(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Orb::Move_Left_Triggered(const FInputActionValue& value)
{
   if (ARobot_Orb* player_pawn = Cast<ARobot_Orb>(GetPawn() ) )
   {
      player_pawn->Move_Left_Triggered(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Orb::Move_Left_Completed(const FInputActionValue& value)
{
   if (ARobot_Orb* player_pawn = Cast<ARobot_Orb>(GetPawn() ) )
   {
      player_pawn->Move_Left_Completed(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Orb::Move_Right_Triggered(const FInputActionValue& value)
{
   if (ARobot_Orb* player_pawn = Cast<ARobot_Orb>(GetPawn() ) )
   {
      player_pawn->Move_Right_Triggered(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Orb::Move_Right_Completed(const FInputActionValue& value)
{
   if (ARobot_Orb* player_pawn = Cast<ARobot_Orb>(GetPawn() ) )
   {
      player_pawn->Move_Right_Completed(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Orb::Jump(const FInputActionValue& value)
{
   if (ARobot_Orb* player_pawn = Cast<ARobot_Orb>(GetPawn() ) )
   {
      player_pawn->Jump(value.Get<bool>());
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Orb::Zoom_Increase(const FInputActionValue& value)
{
   if (ARobot_Orb* player_pawn = Cast<ARobot_Orb>(GetPawn()))
   {
      player_pawn->Zoom_Increase(value.Get<bool>());
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Orb::Zoom_Decrease(const FInputActionValue& value)
{
   if (ARobot_Orb* player_pawn = Cast<ARobot_Orb>(GetPawn()))
   {
      player_pawn->Zoom_Decrease(value.Get<bool>());
   }
}
//------------------------------------------------------------------------------------------------------------
void AController_Robot_Orb::Turn_On_Off_Fleshlight(const FInputActionValue& value)
{
   if (ARobot_Orb* player_pawn = Cast<ARobot_Orb>(GetPawn()))
   {
      player_pawn->Turn_On_Off_Fleshlight(value.Get<bool>());
   }
}
//------------------------------------------------------------------------------------------------------------
