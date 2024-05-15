#include "Main_Menu_Widget.h"
#include "My_Game_Mode.h"

// UMain_Menu_Widget
//------------------------------------------------------------------------------------------------------------
void UMain_Menu_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	Start_Button->OnClicked.AddDynamic(this, &ThisClass::Start_Button_Clicked);
	Exit_Button->OnClicked.AddDynamic(this, &ThisClass::Exit_Button_Clicked);
}
//------------------------------------------------------------------------------------------------------------
void UMain_Menu_Widget::Start_Button_Clicked()
{
	AMy_Game_Mode* game_mode = GetWorld()->GetAuthGameMode<AMy_Game_Mode>();
	if (game_mode)
		game_mode->In_Game_State();
}
//------------------------------------------------------------------------------------------------------------
void UMain_Menu_Widget::Exit_Button_Clicked()
{
	AMy_Game_Mode* game_mode = GetWorld()->GetAuthGameMode<AMy_Game_Mode>();
	if (game_mode)
		game_mode->Close_Game();
}
//------------------------------------------------------------------------------------------------------------
