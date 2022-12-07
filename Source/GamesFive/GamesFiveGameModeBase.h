// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ResourcePickup.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GamesFiveGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class GAMESFIVE_API AGamesFiveGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void StartPlay() override;
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<AResourcePickup> HealthClass;

};
