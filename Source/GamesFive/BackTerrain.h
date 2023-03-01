// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ProceduralMeshComponent.h>
#include "Components/InstancedStaticMeshComponent.h" 
#include "KismetProceduralMeshLibrary.h"
#include "External/FastNoise.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BackTerrain.generated.h"

UCLASS()
class GAMESFIVE_API ABackTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABackTerrain();

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

	void CreateTerrain(FastNoise* noise);
private:
	
	TArray<FVector> CalculateNormals(TArray<FVector> vertices, TArray<int32> triangles);
	void PlaceTrees(FastNoise* noise);
	UMaterialInterface* TerrainMaterial;

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FColor> VertexColours;
	TArray<FProcMeshTangent> Tangents;

	int Scale = 30;
	int SizeX = 30;
	
	TArray<UInstancedStaticMeshComponent*> StaticMeshes;

};
