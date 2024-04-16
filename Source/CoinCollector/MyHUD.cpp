#include "MyHUD.h"
#include "Blueprint/UserWidget.h"

// AMyHUD
//------------------------------------------------------------------------------------------------------------
void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	UUserWidget* user_widget = CreateWidget<UUserWidget>(GetOwningPlayerController(), HUD_Widget);
	if (user_widget)
		user_widget->AddToViewport();
}
//------------------------------------------------------------------------------------------------------------
