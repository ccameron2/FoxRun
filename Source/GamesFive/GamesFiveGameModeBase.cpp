// Copyright Epic Games, Inc. All Rights Reserved.
#include "GamesFiveGameModeBase.h"

#include "External/FastNoise.h"

AGamesFiveGameModeBase::AGamesFiveGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGamesFiveGameModeBase::StartPlay()
{
	Super::StartPlay();

	FVector location = FVector{ 0,0,0 };
	FVector scale = FVector(1, 1, 1);
	FTransform transform;
	transform.SetScale3D(scale);
	transform.SetTranslation(location);

	Seed = FMath::RandRange(0, 69420);


	Noise.SetNoiseType(FastNoise::SimplexFractal);
	Noise.SetSeed(Seed);

	PlayerCharacter = Cast<AThirdPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	TerrainBlocks.Push(GetWorld()->SpawnActor<ATerrainBlock>(TerrainBlockClass, transform));
	TerrainBlocks[0]->PlaceBlocks(&Noise,location);
	BlockIndex++;

	location = FVector{ 0,float(TerrainBlocks[0]->Terrain->SizeY * TerrainBlocks[0]->Terrain->Scale * BlockIndex) - (30 * BlockIndex),0 };
	transform.SetTranslation(location);

	TerrainBlocks.Push(GetWorld()->SpawnActor<ATerrainBlock>(TerrainBlockClass, transform));
	TerrainBlocks[1]->PlaceBlocks(&Noise, location);
	BlockIndex++;

	location = FVector{ 0,float(TerrainBlocks[0]->Terrain->SizeY * TerrainBlocks[0]->Terrain->Scale * BlockIndex) - (30 * BlockIndex),0 };
	transform.SetTranslation(location);

	TerrainBlocks.Push(GetWorld()->SpawnActor<ATerrainBlock>(TerrainBlockClass, transform));
	TerrainBlocks[2]->PlaceBlocks(&Noise, location);
	BlockIndex++;

}

void AGamesFiveGameModeBase::Tick(float DeltaSeconds)
{
	auto scale = TerrainBlocks[0]->Terrain->Scale;
	auto sizeY = TerrainBlocks[0]->Terrain->SizeY;

	if (Generated == false)
	{	
		if (PlayerCharacter->GetActorLocation().Y > scale * sizeY * (BlockIndex - 2))
		{
			if (TerrainBlocks.Num() < 3)
			{
				PlayerCharacter->AddScore(100);
				FVector location = FVector{ 0,float(sizeY * scale * BlockIndex) - (30 * BlockIndex),0 };
				FVector terrainscale = FVector(1, 1, 1);
				FTransform transform;
				transform.SetScale3D(terrainscale);
				transform.SetTranslation(location);

				auto terrainBlock = GetWorld()->SpawnActor<ATerrainBlock>(TerrainBlockClass, transform);
				terrainBlock->PlaceBlocks(&Noise, location);
				TerrainBlocks.Push(terrainBlock);
				BlockIndex++;
				Generated = true;
			}
			else
			{
				TerrainBlocks[0]->Terrain->Destroy();
				TerrainBlocks[0]->LBackTerrain->Destroy();
				TerrainBlocks[0]->RBackTerrain->Destroy();
				TerrainBlocks[0]->Destroy();
				TerrainBlocks.RemoveAt(0);
			}
		}
	}
	else
	{
		if (PlayerCharacter->GetActorLocation().Y > (scale * sizeY * (BlockIndex - 2)) - 2 * scale)
		{
			Generated = false;
		}
	}
	
}