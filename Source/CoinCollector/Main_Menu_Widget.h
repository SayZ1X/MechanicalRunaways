#pragma once

#include "Components/Button.h"
#include "Components/Image.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Main_Menu_Widget.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract)
class COINCOLLECTOR_API UMain_Menu_Widget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget) ) UImage* Background_Image;
	UPROPERTY(meta = (BindWidget) ) UButton* Start_Button;
	UPROPERTY(meta = (BindWidget) ) UButton* Exit_Button;

private:
	void NativeConstruct();

	UFUNCTION() void Start_Button_Clicked();
	UFUNCTION() void Exit_Button_Clicked();
};
//------------------------------------------------------------------------------------------------------------
