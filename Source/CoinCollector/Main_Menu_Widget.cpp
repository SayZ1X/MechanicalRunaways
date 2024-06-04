#include "Main_Menu_Widget.h"
#include "My_Game_Instance.h"

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
	UMy_Game_Instance* game_instance = Cast<UMy_Game_Instance>(GetWorld()->GetGameInstance() );
	if (game_instance)
		game_instance->Hide_Menu();
}
//------------------------------------------------------------------------------------------------------------
void UMain_Menu_Widget::Exit_Button_Clicked()
{
	UMy_Game_Instance* game_instance = Cast<UMy_Game_Instance>(GetWorld()->GetGameInstance() );
	if (game_instance)
		game_instance->Close_Game(); 
}
//------------------------------------------------------------------------------------------------------------
