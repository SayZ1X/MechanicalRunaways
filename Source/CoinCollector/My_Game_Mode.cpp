#include "My_Game_Mode.h"
#include "Blueprint/UserWidget.h"
#include "Base_Player.h"
#include "Base_Coin.h"
#include "My_HUD.h"
#include "My_Game_Instance.h"
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
void AMy_Game_Mode::BeginPlay()
{
   Super::BeginPlay();

   In_Game_State();
}
//------------------------------------------------------------------------------------------------------------
void AMy_Game_Mode::Restart_Level()
{
   UWorld* World = GetWorld();
   if (World)
   {
      FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(World, true);
      FName LevelName(*CurrentLevelName);

      // Логирование имени уровня для отладки
      UE_LOG(LogTemp, Warning, TEXT("Restarting Level: %s"), *CurrentLevelName);

      UGameplayStatics::OpenLevel(World, LevelName);
   }
}
//------------------------------------------------------------------------------------------------------------
void AMy_Game_Mode::Open_Next_Level()
{
   UWorld* World = GetWorld();
   if (World)
   {
      // Получаем имя текущего уровня без префиксов PIE
      FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(World, true);

      // Лог текущего уровня для отладки
      UE_LOG(LogTemp, Warning, TEXT("Current Level: %s"), *CurrentLevelName);

      // Поиск подстроки "LVL_"
      int FoundIndex = CurrentLevelName.Find(TEXT("LVL_"));
      if (FoundIndex != INDEX_NONE)
      {
         // Извлечение числовой части уровня
         FString LevelNumberString = CurrentLevelName.Mid(FoundIndex + 4); // 4 - длина "LVL_"
         if (LevelNumberString.IsNumeric())
         {
            int CurrentLevelIndex = FCString::Atoi(*LevelNumberString);
            int NextLevelIndex = CurrentLevelIndex + 1;

            FString NextLevelName = FString::Printf(TEXT("LVL_%d"), NextLevelIndex);
            FName NextLevelFName(*NextLevelName);

            // Лог следующего уровня для отладки
            UE_LOG(LogTemp, Warning, TEXT("Next Level: %s"), *NextLevelName);

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
void AMy_Game_Mode::In_Game_State()
{
   UMy_Game_Instance* game_instance = Cast<UMy_Game_Instance>(GetWorld()->GetGameInstance() );
   APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
   
   if (player_controller)
   {
      player_controller->SetShowMouseCursor(true);
      player_controller->SetInputMode(FInputModeUIOnly());
      player_controller->DisableInput(player_controller);

      if (!game_instance->isMenu_Open)
      {
         AMy_HUD* HUD = Cast<AMy_HUD>(player_controller->GetHUD());
         if (HUD)
         {
            HUD->In_Game_State();
         }
      }
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
