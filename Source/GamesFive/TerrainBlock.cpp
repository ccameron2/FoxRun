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

//void ATerrainBlock::BeginDestroy()
//{
//	Terrain->Destroy();
//	LBackTerrain->Destroy();
//	RBackTerrain->Destroy();
//}

void ATerrainBlock::PlaceBlocks(FastNoise* noise, FVector location)
{
	FVector spawnlocation = location;
	FVector scale = FVector(1, 1, 1);
	FTransform transform;
	transform.SetScale3D(scale);
	transform.SetTranslation(spawnlocation);

	Terrain = GetWorld()->SpawnActor<ATerrain>(TerrainClass, transform);

	Terrain->CreateLanes(noise);

	spawnlocation = FVector{ LBackgroundXPos,location.Y,0 };
	transform.SetLocation(spawnlocation);

	LBackTerrain = GetWorld()->SpawnActor<ABackTerrain>(BackTerrainClass, transform);
	LBackTerrain->CreateTerrain(noise);

	spawnlocation = FVector{ RBackgroundXPos,location.Y,0 };
	transform.SetLocation(spawnlocation);

	RBackTerrain = GetWorld()->SpawnActor<ABackTerrain>(BackTerrainClass, transform);
	RBackTerrain->CreateTerrain(noise);
}