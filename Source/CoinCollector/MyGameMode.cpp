#include "MyGameMode.h"
#include "Blueprint/UserWidget.h"
#include "BasePlayer.h"
#include "BaseCoin.h"
#include "MyHUD.h"
#include "Kismet/GameplayStatics.h"

// AMyGameMode
//-------------------------------------------------------------------------------------------------------------
AMyGameMode::AMyGameMode() : Super()
{
   ConstructorHelpers::FClassFinder<APawn> main_character(TEXT("/Game/CoinCollector/Blueprints/BP_Player") );
   DefaultPawnClass = main_character.Class;

   ConstructorHelpers::FClassFinder<AMyHUD> HUD_widget(TEXT("/Game/CoinCollector/UI/WB_HUD") );
   HUDClass = HUD_widget.Class;

   Total_Coins = Get_All_Coins();
}
//------------------------------------------------------------------------------------------------------------
int AMyGameMode::Get_All_Coins()
{  // Функция для установки значения количества всех монеток на уровне
   TArray<AActor*> CoinActors;
   UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCoin::StaticClass(), CoinActors);
   int all_coin_counter = CoinActors.Num();
   CoinActors.Empty();
   return all_coin_counter;
}
//-------------------------------------------------------------------------------------------------------------
APawn* AMyGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
   APawn* DefaultPawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

   // Подписываемся на делегат из персонажа
   if (DefaultPawn)
   {
      ABasePlayer* DefaultPlayer = Cast<ABasePlayer>(DefaultPawn);
      if (DefaultPlayer)
      {
         DefaultPlayer->Counter_Collected_Coin_Changed.AddDynamic(this, &AMyGameMode::Check_Win);
      }
   }

   return DefaultPawn;
}
//-------------------------------------------------------------------------------------------------------------
void AMyGameMode::Check_Win(const int new_count_collected_coin)
{
   if (new_count_collected_coin >= Total_Coins)
   {
      UE_LOG(LogTemp, Warning, TEXT("[%S]"), __FUNCTION__);
   }
}
//------------------------------------------------------------------------------------------------------------
