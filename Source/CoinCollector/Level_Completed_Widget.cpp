#include "Level_Completed_Widget.h"
#include "My_Game_Instance.h"
#include "My_Game_Mode.h"
#include "Kismet/GameplayStatics.h"

// ULevel_Completed_Widget
//------------------------------------------------------------------------------------------------------------
void ULevel_Completed_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	Restart_Level_Button->OnClicked.AddDynamic(this, &ThisClass::Restart_Level_Button_Clicked);
	Next_Level_Button->OnClicked.AddDynamic(this, &ThisClass::Next_Level_Button_Clicked);
	Main_Menu_Button->OnClicked.AddDynamic(this, &ThisClass::Main_Menu_Button_Clicked);
}
//------------------------------------------------------------------------------------------------------------
void ULevel_Completed_Widget::Restart_Level_Button_Clicked()
{
	AMy_Game_Mode* game_mode = GetWorld()->GetAuthGameMode<AMy_Game_Mode>();
	if (game_mode)
	{
		game_mode->Restart_Level();
	}
}
//------------------------------------------------------------------------------------------------------------
void ULevel_Completed_Widget::Next_Level_Button_Clicked()
{
	AMy_Game_Mode* game_mode = GetWorld()->GetAuthGameMode<AMy_Game_Mode>();
	if (game_mode)
	{
		game_mode->Open_Next_Level();
	}
}
//------------------------------------------------------------------------------------------------------------
void ULevel_Completed_Widget::Main_Menu_Button_Clicked()
{
	UMy_Game_Instance* game_instance = Cast<UMy_Game_Instance>(GetWorld()->GetGameInstance() );
	if (game_instance)
		game_instance->Show_Menu();
}
//------------------------------------------------------------------------------------------------------------
