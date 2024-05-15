#pragma once

#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Coin_Counter_User_Widget.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract)
class COINCOLLECTOR_API UCoin_Counter_User_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();
	void NativeTick(const FGeometry& my_geometry, float in_delta_time);

	int All_Coins_Count;
	int Count_Collected_Coins;

protected:
	UPROPERTY(meta = (BindWidget) ) UTextBlock* Collected_Coin_Text_Block; // Обязываем создать текстовый виджет для собраных монет в блюпринте
	UPROPERTY(meta = (BindWidget) ) UTextBlock* Timer_Text; // Обязываем создать текстовый виджет для таймера в блюпринте

private:
	int Get_All_Coins();

	UFUNCTION() void Collected_Coin_Changed(const int new_count_collected_coin);

	float Elapsed_Time; // Переменная для таймера
	float Update_Interval; // Переменная для определения с каким интервалом должен обновляться таймер
};
//------------------------------------------------------------------------------------------------------------
