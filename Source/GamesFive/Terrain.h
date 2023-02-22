// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ProceduralMeshComponent.h>
#include "Components/InstancedStaticMeshComponent.h" 
#include "External/FastNoise.h"

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
		float treeNoiseThreshold = 0.8f;
private:
	TArray<FVector> CalculateNormals(TArray<FVector> vertices, TArray<int32> triangles);
	void PlaceTrees(FastNoise* noise);
	TArray<GeometryData> LaneGeometry;

	UMaterialInterface* GrassMaterial;
	UMaterialInterface* DeepGrassMaterial;
	UMaterialInterface* GravelMaterial;

	int Scale = 30;
	int SizeX = 10;
	int LaneXOffset = SizeX - 2;
	int SizeY = 200;

	TArray<UInstancedStaticMeshComponent*> ValleyStaticMeshes;
};
