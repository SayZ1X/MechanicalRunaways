#include "CoinCounterUserWidget.h"
#include "BasePlayer.h"

// UCoinCounterUserWidget
//------------------------------------------------------------------------------------------------------------
void UCoinCounterUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ABasePlayer* own_player = Cast<ABasePlayer>(GetOwningPlayerPawn() );
	if (own_player)
		own_player->Counter_Collected_Coin_Changed.AddDynamic(this, &ThisClass::Collected_Coin_Changed);  // Св-язываем делегат нашего персонажа с функцией для обновления UI текста
}
//------------------------------------------------------------------------------------------------------------
void UCoinCounterUserWidget::Collected_Coin_Changed(const int New_Count_Collected_Coin)
{
	check(Collected_Coin_Text_Block);
	Collected_Coin_Text_Block->SetText(FText::AsNumber(New_Count_Collected_Coin));  // Устанавливаем значения текста в UI в новое значение
}
//------------------------------------------------------------------------------------------------------------
