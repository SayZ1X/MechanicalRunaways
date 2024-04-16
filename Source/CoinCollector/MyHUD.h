#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract)
class COINCOLLECTOR_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void BeginPlay();

private:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> HUD_Widget;
};
//------------------------------------------------------------------------------------------------------------
