// Copyright Epic Games, Inc. All Rights Reserved.
#include "GamesFiveGameModeBase.h"

#include "External/FastNoise.h"

void AGamesFiveGameModeBase::StartPlay()
{
	Super::StartPlay();

	FVector location = FVector{ 0,0,0 };
	FVector scale = FVector(1, 1, 1);
	FTransform transform;
	transform.SetScale3D(scale);
	transform.SetTranslation(location);

	Seed = FMath::RandRange(0, 69420);

	FastNoise noise;
	//noise.SetNoiseType(FastNoise::SimplexFractal);
	//noise.SetSeed(Seed);

	TerrainBlocks.Push(GetWorld()->SpawnActor<ATerrainBlock>(TerrainBlockClass, transform));
	TerrainBlocks[0]->PlaceBlocks(&noise);

}
