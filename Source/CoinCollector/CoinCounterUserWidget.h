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

protected:
	UPROPERTY(meta = (BindWidget) ) UTextBlock* Collected_Coin_Text_Block; // Обязываем создать текстовый виджет в блюпринте

private:
	UFUNCTION() void Collected_Coin_Changed(const int New_Count_Collected_Coin);
};
//------------------------------------------------------------------------------------------------------------
