#pragma once

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

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) void PlayCustomDeath();
	UFUNCTION(BlueprintNativeEvent) void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UStaticMeshComponent* CoinMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float RotationRate;

protected:
	virtual void BeginPlay() override;

	void DeathTimerComplete();

	FTimerHandle DeathTimerHandle;

};
//------------------------------------------------------------------------------------------------------------
