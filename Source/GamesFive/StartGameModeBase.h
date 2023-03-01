// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TerrainBlock.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StartGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMESFIVE_API AStartGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void StartPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
		TArray<ATerrainBlock*> TerrainBlocks;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ATerrainBlock> TerrainBlockClass;

	int BlockIndex = 0;
};
