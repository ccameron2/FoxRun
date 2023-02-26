// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ProceduralMeshComponent.h>
#include "Components/InstancedStaticMeshComponent.h" 
#include "External/FastNoise.h"
#include "Obstacle.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Terrain.generated.h"

// First value repeated at end in generation
UENUM()
enum LaneType
{
	Valley,
	Deep,
	Grass,
	Gravel,
};

struct GeometryData
{
	LaneType Lane;
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FColor> VertexColours;
	TArray<FProcMeshTangent> Tangents;
};

UCLASS()
class GAMESFIVE_API ATerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrain();

	void CreateLanes(FastNoise* noise);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UProceduralMeshComponent* TerrainMesh;

	UPROPERTY(EditAnywhere)
		float treeNoiseThreshold = 0.95f;

	UPROPERTY(EditAnywhere)
		int SizeY = 200;

	UPROPERTY(EditAnywhere)
		float obstacleNoiseThreshold = 0.6f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AObstacle> ObstacleClass;

	int Scale = 30;
	int SizeX = 10;
	int NumLanes = 5;
	void Destroyed() override;

private:
	TArray<FVector> CalculateNormals(TArray<FVector> vertices, TArray<int32> triangles);
	void PlaceTrees(FastNoise* noise);
	void PlaceObstacles(FastNoise* noise, int lane);
	void LoadObstacleModels();
	void LoadTreeModels();
	TArray<GeometryData> LaneGeometry;

	UMaterialInterface* GrassMaterial;
	UMaterialInterface* DeepGrassMaterial;
	UMaterialInterface* GravelMaterial;

	TArray<UStaticMesh*> GrassMeshes;
	TArray<UStaticMesh*> RoadMeshes;
	TArray<UStaticMesh*> WaterMeshes;

	TArray<AObstacle*> GrassObstacles;
	TArray<AObstacle*> RoadObstacles;
	TArray<AObstacle*> WaterObstacles;

	int LaneXOffset = SizeX - 2;
	
	TArray<UInstancedStaticMeshComponent*> ValleyStaticMeshes;
};
