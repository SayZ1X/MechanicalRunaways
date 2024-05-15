#include "My_Player_Controller.h"
#include "Base_Player.h"

// AMy_Player_Controller
//------------------------------------------------------------------------------------------------------------
AMy_Player_Controller::AMy_Player_Controller()
{
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveUp", this, &ThisClass::Move_Up);
	InputComponent->BindAxis("MoveRight", this, &ThisClass::Move_Right);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ThisClass::Jump);
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Move_Up(float value)
{
   if (value != 0.0f)
   {
      ABase_Player* PlayerPawn = Cast<ABase_Player>(GetPawn() );
      if (PlayerPawn)
      {
         PlayerPawn->Move_Up(value);
      }
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Move_Right(float value)
{
   if (value != 0.0f)
   {
      ABase_Player* PlayerPawn = Cast<ABase_Player>(GetPawn() );
      if (PlayerPawn)
      {
         PlayerPawn->Move_Right(value);
      }
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Player_Controller::Jump()
{
   ABase_Player* PlayerPawn = Cast<ABase_Player>(GetPawn() );
   if (PlayerPawn)
   {
      PlayerPawn->Jump();
   }
}
//------------------------------------------------------------------------------------------------------------
