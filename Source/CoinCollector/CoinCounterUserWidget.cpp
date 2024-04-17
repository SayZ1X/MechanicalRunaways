#include "CoinCounterUserWidget.h"
#include "BasePlayer.h"

// UCoinCounterUserWidget
//------------------------------------------------------------------------------------------------------------
void UCoinCounterUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ElapsedTime = 0.0f;
	UpdateInterval = 1.0f;

	ABasePlayer* own_player = Cast<ABasePlayer>(GetOwningPlayerPawn() );
	if (own_player)
		own_player->Counter_Collected_Coin_Changed.AddDynamic(this, &ThisClass::Collected_Coin_Changed);  // Св-язываем делегат нашего персонажа с функцией для обновления UI текста
}
//------------------------------------------------------------------------------------------------------------
void UCoinCounterUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
   Super::NativeTick(MyGeometry, InDeltaTime);

   ElapsedTime += InDeltaTime;
   int minutes = 0;
   int seconds = 0;

   if (ElapsedTime >= UpdateInterval)
   {  // Проверяем, прошло ли достаточно времени для обновления текста

      minutes = FMath::FloorToInt(ElapsedTime / 60);
      seconds = FMath::FloorToInt(FMath::Fmod(ElapsedTime, 60));

      // Обновляем оставшееся время в текстовом блоке
      if (Timer_Text)
      {
         FText TimerText = FText::Format(FText::FromString("{0}:{1}"), FText::AsNumber(minutes), FText::AsNumber(seconds));
         Timer_Text->SetText(TimerText);
      }

      ++UpdateInterval; // Обновляем время когда должен обновиться таймер
   }
}
//------------------------------------------------------------------------------------------------------------
void UCoinCounterUserWidget::Collected_Coin_Changed(const int new_count_collected_coin)
{
	check(Collected_Coin_Text_Block);
	Collected_Coin_Text_Block->SetText(FText::AsNumber(new_count_collected_coin));  // Устанавливаем значения текста в UI в новое значение
}
//------------------------------------------------------------------------------------------------------------
