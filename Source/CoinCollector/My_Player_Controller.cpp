#include "My_Player_Controller.h"
#include "Base_Player.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// AMy_Player_Controller
//------------------------------------------------------------------------------------------------------------
AMy_Player_Controller::AMy_Player_Controller()
{
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::BeginPlay()
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
void AMy_Player_Controller::SetupInputComponent()
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
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Move_Forward_Triggered(const FInputActionValue& value)
{
   if (ABase_Player* player_pawn = Cast<ABase_Player>(GetPawn() ) )
   {
      player_pawn->Move_Forward_Triggered(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Move_Forward_Completed(const FInputActionValue& value)
{
   if (ABase_Player* player_pawn = Cast<ABase_Player>(GetPawn() ) )
   {
      player_pawn->Move_Forward_Completed(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Move_Backward_Triggered(const FInputActionValue& value)
{
   if (ABase_Player* player_pawn = Cast<ABase_Player>(GetPawn() ) )
   {
      player_pawn->Move_Backward_Triggered(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Move_Backward_Completed(const FInputActionValue& value)
{
   if (ABase_Player* player_pawn = Cast<ABase_Player>(GetPawn() ) )
   {
      player_pawn->Move_Backward_Completed(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Move_Left_Triggered(const FInputActionValue& value)
{
   if (ABase_Player* player_pawn = Cast<ABase_Player>(GetPawn() ) )
   {
      player_pawn->Move_Left_Triggered(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Move_Left_Completed(const FInputActionValue& value)
{
   if (ABase_Player* player_pawn = Cast<ABase_Player>(GetPawn() ) )
   {
      player_pawn->Move_Left_Completed(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Move_Right_Triggered(const FInputActionValue& value)
{
   if (ABase_Player* player_pawn = Cast<ABase_Player>(GetPawn() ) )
   {
      player_pawn->Move_Right_Triggered(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Move_Right_Completed(const FInputActionValue& value)
{
   if (ABase_Player* player_pawn = Cast<ABase_Player>(GetPawn() ) )
   {
      player_pawn->Move_Right_Completed(value.Get<bool>() );
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Jump(const FInputActionValue& value)
{
   if (ABase_Player* player_pawn = Cast<ABase_Player>(GetPawn() ) )
   {
      player_pawn->Jump();
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Zoom_Increase(const FInputActionValue& value)
{
   if (ABase_Player* player_pawn = Cast<ABase_Player>(GetPawn()))
   {
      player_pawn->Zoom_Increase();
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Zoom_Decrease(const FInputActionValue& value)
{
   if (ABase_Player* player_pawn = Cast<ABase_Player>(GetPawn()))
   {
      player_pawn->Zoom_Decrease();
   }
}
//------------------------------------------------------------------------------------------------------------
