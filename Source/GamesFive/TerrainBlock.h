// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "External/FastNoise.h"
#include "Terrain.h"
#include "BackTerrain.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainBlock.generated.h"

UCLASS()
class GAMESFIVE_API ATerrainBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrainBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		ATerrain* Terrain;

	UPROPERTY(VisibleAnywhere)
		ABackTerrain* LBackTerrain;

	UPROPERTY(VisibleAnywhere)
		ABackTerrain* RBackTerrain;

	float LBackgroundXPos = 1230;
	float RBackgroundXPos = -1470;


	UPROPERTY(EditAnywhere)
		TSubclassOf<ATerrain> TerrainClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABackTerrain> BackTerrainClass;

	void PlaceBlocks(FastNoise* noise);
};
