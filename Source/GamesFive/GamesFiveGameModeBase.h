// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ResourcePickup.h"
#include "Terrain.h"
#include "BackTerrain.h"

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
	UPROPERTY(EditAnywhere)
		TSubclassOf<ATerrain> TerrainClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABackTerrain> BackTerrainClass;

	UPROPERTY(EditAnywhere)
		ATerrain* Terrain;

	UPROPERTY(EditAnywhere)
		ABackTerrain* LBackTerrain;

	UPROPERTY(EditAnywhere)
		ABackTerrain* RBackTerrain;

	int Seed = 0;
};
