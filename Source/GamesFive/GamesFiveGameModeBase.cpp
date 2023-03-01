// Copyright Epic Games, Inc. All Rights Reserved.
#include "GamesFiveGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	Seed = FMath::RandRange(0, 70000);

	Noise.SetNoiseType(FastNoise::SimplexFractal);
	Noise.SetSeed(Seed);

	PlayerCharacter = Cast<AThirdPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	// Generate initial terrain
	for (int i = 0; i < 4; i++)
	{
		TerrainBlocks.Push(GetWorld()->SpawnActor<ATerrainBlock>(TerrainBlockClass, transform));
		TerrainBlocks[i]->PlaceBlocks(&Noise, location);
		BlockIndex++;

		location = FVector{ 0,float(TerrainBlocks[0]->Terrain->SizeY * TerrainBlocks[0]->Terrain->Scale * BlockIndex) - (30 * BlockIndex),0 };
		transform.SetTranslation(location);
	}
}

void AGamesFiveGameModeBase::Tick(float DeltaSeconds)
{
	PlayerCharacter = Cast<AThirdPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!PlayerCharacter) return;

	Score = PlayerCharacter->GetScore();

	// Save player location on first tick (cant be done in startplay as character spawned later)
	if (FirstTick) { LastPlayerLocation = PlayerCharacter->GetActorLocation(); FirstTick = false; }
	static auto scale = TerrainBlocks[0]->Terrain->Scale;
	static auto sizeY = TerrainBlocks[0]->Terrain->SizeY;
	static int numLanes = TerrainBlocks[0]->Terrain->NumLanes;
	static int laneSizeX = TerrainBlocks[0]->Terrain->SizeX;

	// Generation
	if (Generated == false)
	{	
		if (PlayerCharacter->GetActorLocation().Y > (scale * sizeY * (BlockIndex - 2)) + (5 * scale))
		{
			if (TerrainBlocks.Num() < 4)
			{
				// Increase character speed and increment movement speed
				PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed += 80;
				PlayerCharacter->AddScore(100);
				
				// Spawn Params
				FVector location = FVector{ 0,float(sizeY * scale * BlockIndex) - (30 * BlockIndex),0 };
				FVector terrainscale = FVector(1, 1, 1);
				FTransform transform;
				transform.SetScale3D(terrainscale);
				transform.SetTranslation(location);

				// Spawn a new terrain block and increment block index
				auto terrainBlock = GetWorld()->SpawnActor<ATerrainBlock>(TerrainBlockClass, transform);
				terrainBlock->PlaceBlocks(&Noise, location, BlockIndex / 100);
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
		// If player is slighty into the second block
		if (PlayerCharacter->GetActorLocation().Y > (scale * sizeY * (BlockIndex - 2)) - 2 * scale)
		{
			Generated = false;
		}
	}
	
	// Keep player within the lanes
	if (PlayerCharacter->GetActorLocation().X > ((numLanes - 2) * (laneSizeX * scale)) + 0.5 * laneSizeX * scale)
	{
		LastPlayerLocation += FVector{ float(-scale / 2),0,0 };
		PlayerCharacter->SetActorLocation(LastPlayerLocation);
	}
	else if (PlayerCharacter->GetActorLocation().X < (laneSizeX * scale))
	{
		LastPlayerLocation += FVector{ float(scale / 2),0,0 };
		PlayerCharacter->SetActorLocation(LastPlayerLocation);
	}
	LastPlayerLocation = PlayerCharacter->GetActorLocation();	
}