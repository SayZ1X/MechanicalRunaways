#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "My_Game_Instance.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS()
class COINCOLLECTOR_API UMy_Game_Instance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMy_Game_Instance();

	virtual void OnStart();

	UFUNCTION() void Show_Menu();
	UFUNCTION() void Hide_Menu();
	UFUNCTION() void Close_Game();
	UFUNCTION() void Open_UI();
	UFUNCTION() void Close_UI();

	bool isMenu_Open;

protected:
	//TArray<FName> Level_Names;
	UFUNCTION() void Check_UI();

	int Count_UI;
	UPROPERTY() UUserWidget* Main_Menu_Widget;
	UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UUserWidget> Mein_Menu_Widget_Class;
};
//------------------------------------------------------------------------------------------------------------
