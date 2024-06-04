#include "My_HUD.h"
#include "Blueprint/UserWidget.h"
#include "My_Game_Mode.h"
#include "My_Game_Instance.h"

// AMyHUD
//------------------------------------------------------------------------------------------------------------
void AMy_HUD::BeginPlay()
{
	Super::BeginPlay();

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
			UserWidget->RemoveFromViewport();
			UserWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), Win_HUD_Widget);
			if (UserWidget)
			{
				UserWidget->AddToViewport();
				UMy_Game_Instance* game_instance = Cast<UMy_Game_Instance>(GetWorld()->GetGameInstance() );
				if (game_instance)
					game_instance->Open_UI();
			}
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AMy_HUD::In_Game_State()
{
	if (UserWidget == nullptr)
	{
		//UserWidget->RemoveFromViewport();
		UserWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), Default_HUD_Widget);
		if (UserWidget)
		{
			UserWidget->AddToViewport();
			UMy_Game_Instance* game_instance = Cast<UMy_Game_Instance>(GetWorld()->GetGameInstance());
			if (game_instance)
				game_instance->Close_UI();
		}
	}
}
//------------------------------------------------------------------------------------------------------------
//void AMy_HUD::Level_Completed_State()
//{
//	if (UserWidget)
//	{
//		UserWidget->RemoveFromViewport();
//		UserWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), Win_HUD_Widget);
//		if (UserWidget)
//			UserWidget->AddToViewport();
//	}
//}
//------------------------------------------------------------------------------------------------------------
