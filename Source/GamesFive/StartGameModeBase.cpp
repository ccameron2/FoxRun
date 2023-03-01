// Fill out your copyright notice in the Description page of Project Settings.
#include "StartGameModeBase.h"


void AStartGameModeBase::StartPlay()
{
	Super::StartPlay();
	FVector location = FVector{ 0,0,0 };
	FVector scale = FVector(1, 1, 1);
	FTransform transform;
	transform.SetScale3D(scale);
	transform.SetTranslation(location);

	int Seed = FMath::RandRange(0, 70000);

	FastNoise Noise;
	Noise.SetNoiseType(FastNoise::SimplexFractal);
	Noise.SetSeed(Seed);

	// Generate terrain for title screen
	for (int i = 0; i < 4; i++)
	{
		TerrainBlocks.Push(GetWorld()->SpawnActor<ATerrainBlock>(TerrainBlockClass, transform));
		TerrainBlocks[i]->PlaceBlocks(&Noise, location);
		BlockIndex++;

		location = FVector{ 0,float(TerrainBlocks[0]->Terrain->SizeY * TerrainBlocks[0]->Terrain->Scale * BlockIndex) - (30 * BlockIndex),0 };
		transform.SetTranslation(location);
	}
}
