#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "My_HUD.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract)
class COINCOLLECTOR_API AMy_HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay();

	UFUNCTION() void In_Game_State();
	//UFUNCTION() void Level_Completed_State();

private:
	UFUNCTION() void Change_On_Win(bool is_win);

	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> Default_HUD_Widget;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> Win_HUD_Widget;
	UPROPERTY() UUserWidget* UserWidget;

};
//------------------------------------------------------------------------------------------------------------
