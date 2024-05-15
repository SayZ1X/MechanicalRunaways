#pragma once

#include "Components/Button.h"
#include "Components/Image.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Level_Completed_Widget.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract)
class COINCOLLECTOR_API ULevel_Completed_Widget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget) ) UImage* BG_Image;
	UPROPERTY(meta = (BindWidget) ) UButton* Restart_Level_Button;
	UPROPERTY(meta = (BindWidget) ) UButton* Next_Level_Button;
	UPROPERTY(meta = (BindWidget) ) UButton* Main_Menu_Button;

private:
	virtual void NativeConstruct();

	UFUNCTION() void Restart_Level_Button_Clicked();
	UFUNCTION() void Next_Level_Button_Clicked();
	UFUNCTION() void Main_Menu_Button_Clicked();
};
//------------------------------------------------------------------------------------------------------------
