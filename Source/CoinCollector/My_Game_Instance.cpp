#include "My_Game_Instance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "My_HUD.h"

// UMy_Game_Instance
//------------------------------------------------------------------------------------------------------------
UMy_Game_Instance::UMy_Game_Instance() : Count_UI(0)
{
   ConstructorHelpers::FClassFinder<UUserWidget> Main_Menu_BP_Class(TEXT("/Game/CoinCollector/UI/WB_Main_Menu_UI"));
   
   if (Main_Menu_BP_Class.Class != nullptr)
   {
      Mein_Menu_Widget_Class = Main_Menu_BP_Class.Class;
   }

   Main_Menu_Widget = nullptr;
}
//------------------------------------------------------------------------------------------------------------
void UMy_Game_Instance::OnStart()
{
   Super::OnStart();

   Show_Menu();
}
//-------------------------------------------------------------------------------------------------------------
void UMy_Game_Instance::Show_Menu()
{
   if (Mein_Menu_Widget_Class != nullptr && Main_Menu_Widget == nullptr)
   {
      Main_Menu_Widget = CreateWidget<UUserWidget>(this, Mein_Menu_Widget_Class);
      if (Main_Menu_Widget != nullptr)
      {
         Main_Menu_Widget->AddToViewport();

         Open_UI();

         isMenu_Open = true;
      }
   }
}
//------------------------------------------------------------------------------------------------------------
void UMy_Game_Instance::Hide_Menu()
{
   if (Main_Menu_Widget != nullptr)
   {
      Main_Menu_Widget->RemoveFromViewport();
      Main_Menu_Widget = nullptr;

      APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
      if (player_controller)
      {
         isMenu_Open = false;

         Close_UI();

         AMy_HUD* HUD = Cast<AMy_HUD>(player_controller->GetHUD() );
         if (HUD)
         {
            HUD->In_Game_State();
         }
      }
   }
}
//------------------------------------------------------------------------------------------------------------
void UMy_Game_Instance::Close_Game()
{
   UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
//------------------------------------------------------------------------------------------------------------
void UMy_Game_Instance::Open_UI()
{
   ++Count_UI;
   Check_UI();
}
//------------------------------------------------------------------------------------------------------------
void UMy_Game_Instance::Close_UI()
{
   if (Count_UI > 0)
   {
      --Count_UI;
      Check_UI();
   }
}
//------------------------------------------------------------------------------------------------------------
void UMy_Game_Instance::Check_UI()
{
   APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
   if (player_controller)
   {
      if (Count_UI <= 0)
      {
         player_controller->bShowMouseCursor = false;
         UE_LOG(LogTemp, Warning, TEXT("[%S] >>>>> SetShowMouseCursor State = false"), __FUNCTION__);
      }
      else
      {
         player_controller->bShowMouseCursor = true;
         UE_LOG(LogTemp, Warning, TEXT("[%S] >>>>> SetShowMouseCursor State = true"), __FUNCTION__);
      }
   }
}
//------------------------------------------------------------------------------------------------------------
