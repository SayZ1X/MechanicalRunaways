#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Coin_Counter_User_Widget.h"
#include "My_Game_Mode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCheck_Level_Win, bool, Level_Completed);

//------------------------------------------------------------------------------------------------------------
UCLASS()
class COINCOLLECTOR_API AMy_Game_Mode : public AGameModeBase
{
   GENERATED_BODY()

public:
   AMy_Game_Mode();

   virtual void BeginPlay();

   void Restart_Level();
   void Open_Next_Level();

   UFUNCTION() void Check_Win(const int new_count_collected_coin);
   UFUNCTION() void In_Game_State();

   UPROPERTY(BlueprintAssignable) FCheck_Level_Win Check_Level_Win;

protected:
   APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot);
   int Get_All_Coins();

   TArray<FName> Level_Names;
   int Total_Coins;
};
//------------------------------------------------------------------------------------------------------------
