#pragma once

#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoinCounterUserWidget.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract)
class COINCOLLECTOR_API UCoinCounterUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	int All_Coins_Count;
	int Count_Collected_Coins;

protected:

	UPROPERTY(meta = (BindWidget) ) UTextBlock* Collected_Coin_Text_Block; // Обязываем создать текстовый виджет для собраных монет в блюпринте
	UPROPERTY(meta = (BindWidget) ) UTextBlock* Timer_Text; // Обязываем создать текстовый виджет для таймера в блюпринте

private:
	float ElapsedTime; // Переменная для таймера
	float UpdateInterval; // Переменная для определения с каким интервалом должен обновляться таймер

	int Get_All_Coins();

	UFUNCTION() void Collected_Coin_Changed(const int new_count_collected_coin);
};
//------------------------------------------------------------------------------------------------------------
