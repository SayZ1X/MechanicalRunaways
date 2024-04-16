#pragma once

#include "Sound/SoundBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "BaseCoin.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS()
class COINCOLLECTOR_API ABaseCoin : public AActor
{
	GENERATED_BODY()

public:
	ABaseCoin();

	virtual void Tick(float delta_time);

	bool Was_Collected;

	UFUNCTION(BlueprintCallable) void Play_Custom_Death();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin_Setting") USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin_Setting") UStaticMeshComponent* Coin_Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coin_Setting") USoundBase* Coin_Sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin_Setting") float Rotation_Rate;

protected:
	void Death_Timer_Complete();

	FTimerHandle Death_Timer_Handle;

};
//------------------------------------------------------------------------------------------------------------
