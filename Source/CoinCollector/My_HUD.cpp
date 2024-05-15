#include "My_HUD.h"
#include "Blueprint/UserWidget.h"
#include "My_Game_Mode.h"

// AMyHUD
//------------------------------------------------------------------------------------------------------------
void AMy_HUD::BeginPlay()
{
	Super::BeginPlay();
	UserWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), HUD_Main_Menu);
	if (UserWidget)
		UserWidget->AddToViewport();

	AGameModeBase* game_mode = GetWorld()->GetAuthGameMode();
	if (game_mode)
	{
		AMy_Game_Mode* my_game_mode = Cast<AMy_Game_Mode>(game_mode);
		my_game_mode->Check_Level_Win.AddDynamic(this, &ThisClass::Change_On_Win);
	}
}
//------------------------------------------------------------------------------------------------------------
void AMy_HUD::Change_On_Win(bool is_win)
{
	if (is_win)
	{
		if (UserWidget)
		{
			APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
			if (player_controller)
				player_controller->SetShowMouseCursor(true);
			
			UserWidget->RemoveFromViewport();
			UserWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), Win_HUD_Widget);
			if (UserWidget)
				UserWidget->AddToViewport();
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AMy_HUD::Menu_State()
{
	if (UserWidget)
	{
		UserWidget->RemoveFromViewport();
		UserWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), HUD_Main_Menu);
		if (UserWidget)
			UserWidget->AddToViewport();
	}
}
//------------------------------------------------------------------------------------------------------------
void AMy_HUD::In_Game_State()
{
	if (UserWidget)
	{
		UserWidget->RemoveFromViewport();
		UserWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), Default_HUD_Widget);
		if (UserWidget)
			UserWidget->AddToViewport();
	}
}
//------------------------------------------------------------------------------------------------------------
void AMy_HUD::Level_Completed_State()
{
	if (UserWidget)
	{
		UserWidget->RemoveFromViewport();
		UserWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), Win_HUD_Widget);
		if (UserWidget)
			UserWidget->AddToViewport();
	}
}
//------------------------------------------------------------------------------------------------------------
