// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ResourcePickup.h"
#include "TerrainBlock.h"

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
	UPROPERTY(VisibleAnywhere)
		TArray<ATerrainBlock*> TerrainBlocks;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ATerrainBlock> TerrainBlockClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AResourcePickup> HealthClass;

	int Seed = 0;

};
