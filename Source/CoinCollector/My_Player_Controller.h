#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "My_Player_Controller.generated.h"

class ABase_Player;

//------------------------------------------------------------------------------------------------------------
UCLASS()
class COINCOLLECTOR_API AMy_Player_Controller : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMy_Player_Controller();

protected:
	virtual void SetupInputComponent();

	void Move_Up(float value);
	void Move_Right(float value);
	void Jump();

};
//------------------------------------------------------------------------------------------------------------
