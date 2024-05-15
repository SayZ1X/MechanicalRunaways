#include "My_Game_Mode.h"
#include "Blueprint/UserWidget.h"
#include "Base_Player.h"
#include "Base_Coin.h"
#include "My_HUD.h"
#include "Kismet/GameplayStatics.h"

// AMy_Game_Mode
//-------------------------------------------------------------------------------------------------------------
AMy_Game_Mode::AMy_Game_Mode() : Super()
{
   ConstructorHelpers::FClassFinder<APawn> main_character(TEXT("/Game/CoinCollector/Blueprints/BP_Player") );
   DefaultPawnClass = main_character.Class;

   ConstructorHelpers::FClassFinder<AMy_HUD> HUD_widget(TEXT("/Game/CoinCollector/UI/BP_HUD") );
   HUDClass = HUD_widget.Class;

   Level_Names = {
      "LVL_0",
      "LVL_1",
      "LVL_2"
   };

   Total_Coins = Get_All_Coins();
}
//------------------------------------------------------------------------------------------------------------
void AMy_Game_Mode::Open_Next_Level()
{
   UWorld* World = GetWorld();
   if (World)
   {
      // Получаем имя текущего уровня
      FString CurrentLevelName = World->GetMapName();

      // В режиме PIE (Play In Editor) имя уровня будет иметь префикс "UEDPIE_" и суффикс с индексом PIE
      FString ShortLevelName = CurrentLevelName;
      if (CurrentLevelName.StartsWith("UEDPIE_"))
      {
         // Убираем префикс "UEDPIE_"
         ShortLevelName = CurrentLevelName.RightChop(CurrentLevelName.Find(TEXT("_")) + 1);
      }

      // Лог текущего уровня для отладки
      UE_LOG(LogTemp, Warning, TEXT("ShortLevelName: %s"), *ShortLevelName);

      // Поиск подстроки "LVL_"
      int32 FoundIndex = ShortLevelName.Find(TEXT("LVL_"));
      if (FoundIndex != INDEX_NONE)
      {
         // Извлечение числовой части уровня
         FString LevelNumberString = ShortLevelName.Mid(FoundIndex + 4); // 4 - длина "LVL_"
         if (LevelNumberString.IsNumeric())
         {
            int32 CurrentLevelIndex = FCString::Atoi(*LevelNumberString);
            int32 NextLevelIndex = CurrentLevelIndex + 1;

            FString NextLevelName = FString::Printf(TEXT("LVL_%d"), NextLevelIndex);
            FName NextLevelFName(*NextLevelName);

            // Лог следующего уровня для отладки
            UE_LOG(LogTemp, Warning, TEXT("NextLevelName: %s"), *NextLevelName);

            // Загружаем следующий уровень
            UGameplayStatics::OpenLevel(World, NextLevelFName);
            return;
         }
      }

      UE_LOG(LogTemp, Warning, TEXT("Current level name format is incorrect or cannot determine next level."));
   }
}
//-------------------------------------------------------------------------------------------------------------
void AMy_Game_Mode::Check_Win(const int new_count_collected_coin)
{
   if (new_count_collected_coin >= Total_Coins)
   {
      UE_LOG(LogTemp, Warning, TEXT("[%S]"), __FUNCTION__);
      Check_Level_Win.Broadcast(true);
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Game_Mode::Menu_State()
{
   APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
   if (player_controller)
   {
      player_controller->SetShowMouseCursor(true);

      AMy_HUD* HUD = Cast<AMy_HUD>(player_controller->GetHUD() );
      if (HUD)
      {
         HUD->Menu_State();
      }
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Game_Mode::In_Game_State()
{
   APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
   if (player_controller)
   {
      player_controller->SetShowMouseCursor(false);
      player_controller->bShowMouseCursor = false;

      AMy_HUD* HUD = Cast<AMy_HUD>(player_controller->GetHUD() );
      if (HUD)
      {
         HUD->In_Game_State();
      }
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Game_Mode::Close_Game()
{
   APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
   if (PlayerController)
   {
      PlayerController->ConsoleCommand("quit");
   }
}
//------------------------------------------------------------------------------------------------------------
APawn* AMy_Game_Mode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
   APawn* DefaultPawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

   // Подписываемся на делегат из персонажа
   if (DefaultPawn)
   {
      ABase_Player* DefaultPlayer = Cast<ABase_Player>(DefaultPawn);
      if (DefaultPlayer)
      {
         DefaultPlayer->Counter_Collected_Coin_Changed.AddDynamic(this, &ThisClass::Check_Win);
      }
   }

   return DefaultPawn;
}
//------------------------------------------------------------------------------------------------------------
int AMy_Game_Mode::Get_All_Coins()
{  // Функция для установки значения количества всех монеток на уровне
   TArray<AActor*> CoinActors;
   UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABase_Coin::StaticClass(), CoinActors);
   int all_coin_counter = CoinActors.Num();
   CoinActors.Empty();
   return all_coin_counter;
}
//-------------------------------------------------------------------------------------------------------------
