// Copyright Epic Games, Inc. All Rights Reserved.
#include "GamesFiveGameModeBase.h"

void AGamesFiveGameModeBase::StartPlay()
{
	Super::StartPlay();

	FActorSpawnParameters SpawnParams;
	FVector location = FVector{ 0,0,0 };
	FVector scale = FVector(1, 1, 1);
	FTransform transform;
	transform.SetScale3D(scale);
	transform.SetTranslation(location);

	Terrain = GetWorld()->SpawnActor<ATerrain>(TerrainClass, transform);

	FastNoise noise;
	noise.SetNoiseType(FastNoise::SimplexFractal);
	noise.SetSeed(Seed);
	Terrain->CreateLanes(&noise);

	location = FVector{ 1230,0,0 };
	transform.SetLocation(location);

	LBackTerrain = GetWorld()->SpawnActor<ABackTerrain>(BackTerrainClass, transform);
	LBackTerrain->CreateTerrain(&noise);

	location = FVector{ -1470.0,0,0 };
	transform.SetLocation(location);

	RBackTerrain = GetWorld()->SpawnActor<ABackTerrain>(BackTerrainClass, transform);
	RBackTerrain->CreateTerrain(&noise);
}
