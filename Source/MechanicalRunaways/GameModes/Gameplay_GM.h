// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Net/UnrealNetwork.h"
#include "Gameplay_GM.generated.h"

/**
 * 
 */
UCLASS()
class MECHANICALRUNAWAYS_API AGameplay_GM : public AGameMode
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void Multicast_Robot_Fly_Move(bool is_forward_backward, bool is_negative_axis);
	
};
