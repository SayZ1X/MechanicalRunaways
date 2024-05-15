#include "Coin_Counter_User_Widget.h"
#include "Base_Player.h"
#include "Base_Coin.h"
#include "Kismet/GameplayStatics.h"

// UCoin_Counter_User_Widget
//------------------------------------------------------------------------------------------------------------
void UCoin_Counter_User_Widget::NativeConstruct()
{
	Super::NativeConstruct();

   Elapsed_Time = 0.0f;  // Устанавливаем базовое значение таймера
   Update_Interval = 1.0f;  // Устанавливаем интервал обновления таймера

   All_Coins_Count = Get_All_Coins();  // Устанавливаем значение количества всех монет на уровне
   Count_Collected_Coins = 0;  // Устанавливаем значение количества собраных монет на уровне
   Collected_Coin_Changed(0);  // Вызываем обновление текста в виджете с базовым значением собраных монет

	ABase_Player* own_player = Cast<ABase_Player>(GetOwningPlayerPawn() );
	if (own_player)
		own_player->Counter_Collected_Coin_Changed.AddDynamic(this, &ThisClass::Collected_Coin_Changed);  // Связываем делегат нашего персонажа с функцией для обновления UI текста
}
//------------------------------------------------------------------------------------------------------------
void UCoin_Counter_User_Widget::NativeTick(const FGeometry& my_geometry, float in_delta_time)
{
   Super::NativeTick(my_geometry, in_delta_time);

   Elapsed_Time += in_delta_time;
   int minutes = 0;
   int seconds = 0;

   if (Elapsed_Time >= Update_Interval)
   {  // Проверяем, прошло ли достаточно времени для обновления текста

      minutes = FMath::FloorToInt(Elapsed_Time / 60);
      seconds = FMath::FloorToInt(FMath::Fmod(Elapsed_Time, 60));

      // Обновляем оставшееся время в текстовом блоке
      if (Timer_Text)
      {
         FText TimerText = FText::Format(FText::FromString("{0}:{1}"), FText::AsNumber(minutes), FText::AsNumber(seconds));
         Timer_Text->SetText(TimerText);
      }

      ++Update_Interval; // Обновляем время когда должен обновиться таймер
   }
}
//------------------------------------------------------------------------------------------------------------
int UCoin_Counter_User_Widget::Get_All_Coins()
{  // Функция для установки значения количества всех монеток на уровне
   TArray<AActor*> coin_actors;
   UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABase_Coin::StaticClass(), coin_actors);
   int all_coin_counter = coin_actors.Num();
   coin_actors.Empty();
   return all_coin_counter;
}
//------------------------------------------------------------------------------------------------------------
void UCoin_Counter_User_Widget::Collected_Coin_Changed(const int new_count_collected_coin)
{
	check(Collected_Coin_Text_Block);
   Count_Collected_Coins++;
   FText coin_text = FText::Format(FText::FromString("{0}/{1}"), FText::AsNumber(new_count_collected_coin), FText::AsNumber(All_Coins_Count));
	Collected_Coin_Text_Block->SetText(coin_text);  // Устанавливаем значения текста в UI в новое значение
}
//------------------------------------------------------------------------------------------------------------
