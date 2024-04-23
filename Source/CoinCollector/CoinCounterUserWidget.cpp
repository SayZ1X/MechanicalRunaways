#include "CoinCounterUserWidget.h"
#include "BasePlayer.h"
#include "BaseCoin.h"
#include "Kismet/GameplayStatics.h"

// UCoinCounterUserWidget
//------------------------------------------------------------------------------------------------------------
void UCoinCounterUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

   ElapsedTime = 0.0f;  // Устанавливаем базовое значение таймера
   UpdateInterval = 1.0f;  // Устанавливаем интервал обновления таймера

   All_Coins_Count = Get_All_Coins();  // Устанавливаем значение количества всех монет на уровне
   Count_Collected_Coins = 0;  // Устанавливаем значение количества собраных монет на уровне
   Collected_Coin_Changed(0);  // Вызываем обновление текста в виджете с базовым значением собраных монет

	ABasePlayer* own_player = Cast<ABasePlayer>(GetOwningPlayerPawn() );
	if (own_player)
		own_player->Counter_Collected_Coin_Changed.AddDynamic(this, &ThisClass::Collected_Coin_Changed);  // Связываем делегат нашего персонажа с функцией для обновления UI текста
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
int UCoinCounterUserWidget::Get_All_Coins()
{  // Функция для установки значения количества всех монеток на уровне
   TArray<AActor*> CoinActors;
   UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCoin::StaticClass(), CoinActors);
   int all_coin_counter = CoinActors.Num();
   CoinActors.Empty();
   return all_coin_counter;
}
//------------------------------------------------------------------------------------------------------------
void UCoinCounterUserWidget::Collected_Coin_Changed(const int new_count_collected_coin)
{
	check(Collected_Coin_Text_Block);
   Count_Collected_Coins++;
   FText coin_text = FText::Format(FText::FromString("{0}/{1}"), FText::AsNumber(new_count_collected_coin), FText::AsNumber(All_Coins_Count));
	Collected_Coin_Text_Block->SetText(coin_text);  // Устанавливаем значения текста в UI в новое значение
}
//------------------------------------------------------------------------------------------------------------
