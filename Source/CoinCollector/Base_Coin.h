#pragma once

#include "Sound/SoundBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Base_Coin.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS()
class COINCOLLECTOR_API ABase_Coin : public AActor
{
	GENERATED_BODY()

public:
	ABase_Coin();

	virtual void Tick(float delta_time);

	UFUNCTION(BlueprintCallable) void Play_Custom_Death();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin_Setting") USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin_Setting") UStaticMeshComponent* Coin_Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coin_Setting") USoundBase* Coin_Sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin_Setting") float Rotation_Rate;

	bool Was_Collected;

protected:
	void Death_Timer_Complete();

	FTimerHandle Death_Timer_Handle;

};
//------------------------------------------------------------------------------------------------------------
