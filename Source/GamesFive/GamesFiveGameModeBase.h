// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TerrainBlock.h"
#include "ThirdPersonCharacter.h"
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
public:
		AGamesFiveGameModeBase();
protected:
	virtual void StartPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadOnly)
		int Score;
private:
	UPROPERTY(VisibleAnywhere)
		TArray<ATerrainBlock*> TerrainBlocks;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ATerrainBlock> TerrainBlockClass;

	AThirdPersonCharacter* PlayerCharacter;
	FastNoise Noise;
	FVector LastPlayerLocation;
	int Seed = 0;
	bool Generated = false;
	int BlockIndex = 0;
	bool FirstTick = true;
};
