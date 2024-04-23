#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

class UCoinCounterUserWidget;
class ABasePlayer;

//------------------------------------------------------------------------------------------------------------
UCLASS()
class COINCOLLECTOR_API AMyGameMode : public AGameModeBase
{
   GENERATED_BODY()

public:
   AMyGameMode();

   UFUNCTION() void Check_Win(const int new_count_collected_coin);

protected:
   int Total_Coins;

   int Get_All_Coins();

   APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot);
};
//------------------------------------------------------------------------------------------------------------
