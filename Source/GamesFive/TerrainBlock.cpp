// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainBlock.h"

// Sets default values
ATerrainBlock::ATerrainBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATerrainBlock::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATerrainBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATerrainBlock::PlaceBlocks(FastNoise* noise, FVector location, float obsNoiseThresIncr)
{
	// Spawn params
	FVector spawnlocation = location;
	FVector scale = FVector(1, 1, 1);
	FTransform transform;
	transform.SetScale3D(scale);
	transform.SetTranslation(spawnlocation);

	// Spawn main terrain
	Terrain = GetWorld()->SpawnActor<ATerrain>(TerrainClass, transform);

	// Increase amount of obstacles slightly
	if (Terrain->obstacleNoiseThreshold - obsNoiseThresIncr > MaxObsThreshold)
	{
		Terrain->obstacleNoiseThreshold -= obsNoiseThresIncr;
	}
	
	// Generate terrain
	Terrain->CreateLanes(noise);

	// Spawn background actors and generate terrain
	spawnlocation = FVector{ LBackgroundXPos,location.Y,0 };
	transform.SetLocation(spawnlocation);

	LBackTerrain = GetWorld()->SpawnActor<ABackTerrain>(BackTerrainClass, transform);
	LBackTerrain->CreateTerrain(noise);

	spawnlocation = FVector{ RBackgroundXPos,location.Y,0 };
	transform.SetLocation(spawnlocation);

	RBackTerrain = GetWorld()->SpawnActor<ABackTerrain>(BackTerrainClass, transform);
	RBackTerrain->CreateTerrain(noise);
}