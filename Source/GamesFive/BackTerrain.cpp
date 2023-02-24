// Fill out your copyright notice in the Description page of Project Settings.
#include "BackTerrain.h"

// Sets default values
ABackTerrain::ABackTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TerrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Terrain Mesh"));
	SetRootComponent(TerrainMesh);

	ConstructorHelpers::FObjectFinder<UMaterialInstance> grassMaterial(TEXT("M_Terrain_DeepGrass'/Game/Materials/M_Terrain_DeepGrass.M_Terrain_DeepGrass'"));
	TerrainMaterial = grassMaterial.Object;

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset1(TEXT("StaticMesh'/Game/Models/Nature/CommonTree_2'"));
	StaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree1 Static Mesh")));
	StaticMeshes[0]->SetStaticMesh(MeshAsset1.Object);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset2(TEXT("StaticMesh'/Game/Models/Nature/PineTree_3'"));
	StaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree2 Static Mesh")));
	StaticMeshes[1]->SetStaticMesh(MeshAsset2.Object);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset3(TEXT("StaticMesh'/Game/Models/Nature/CommonTree_5'"));
	StaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree3 Static Mesh")));
	StaticMeshes[2]->SetStaticMesh(MeshAsset3.Object);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset4(TEXT("StaticMesh'/Game/Models/Nature/CommonTree_4'"));
	StaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree4 Static Mesh")));
	StaticMeshes[3]->SetStaticMesh(MeshAsset4.Object);
}

// Called when the game starts or when spawned
void ABackTerrain::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABackTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABackTerrain::CreateTerrain(FastNoise* noise)
{
	Vertices.Init({ 0,0,0 }, SizeX * SizeY);

	// Create a grid of vertices
	int indexX = 0;
	for (int j = 0; j < SizeX; j++)
	{
		int indexY = 0;
		for (int k = 0; k < SizeY; k++)
		{
			FVector newVector = FVector{ float(j * Scale), float(k * Scale),0 };
			Vertices[SizeY * indexX + indexY] = newVector;
			indexY++;
		}
		indexX++;
	}
	// Generate triangles for grid of vertices
	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(SizeX, SizeY, false, Triangles);

	// Store the tallest vector index and height
	float tallestVectorHeight = 0;
	int tallestVector = 0;

	// For each vertex, get 2 different noise values and apply them to vertex height at different scales.
	for (int j = 0; j < Vertices.Num(); j++)
	{
		// Get input vector from vertex list and sample noise at different levels
		auto input = Vertices[j];
		auto result1 = noise->GetNoise((input.X + GetActorLocation().X) / 300, (input.Y + GetActorLocation().Y) / 300);
		Vertices[j].Z += result1 * 200;
		auto result2 = noise->GetNoise((input.X + GetActorLocation().X) / 20, (input.Y + GetActorLocation().Y) / 20);
		Vertices[j].Z += result2 * 100;

		// Find the tallest vector and store in variables
		if (Vertices[j].Z > tallestVectorHeight)
		{
			tallestVector = j;
			tallestVectorHeight = Vertices[j].Z;
		}
	}

	Normals = CalculateNormals(Vertices, Triangles);

	TerrainMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColours, Tangents, true);
	TerrainMesh->SetMaterial(0, TerrainMaterial);

	PlaceTrees(noise);
}

TArray<FVector> ABackTerrain::CalculateNormals(TArray<FVector> vertices, TArray<int32> triangles)
{
	TArray<FVector> normals;
	normals.Init({ 0,0,0 }, vertices.Num());

	for (int i = 0; i < triangles.Num() - 3; i += 3)
	{
		auto a = vertices[triangles[i]];
		auto b = vertices[triangles[i + 1]];
		auto c = vertices[triangles[i + 2]];

		auto v1 = a - b;
		auto v2 = c - b;
		auto n = v1 ^ v2;
		n.Normalize();

		normals[triangles[i]] += n;
		normals[triangles[i + 1]] += n;
		normals[triangles[i + 2]] += n;
	}

	for (auto& norm : normals)
	{
		norm.Normalize();
	}
	return normals;
}

void ABackTerrain::PlaceTrees(FastNoise* noise)
{

	for (auto& vertex : Vertices)
	{
		float treeNoise = noise->GetNoise((vertex.X + GetActorLocation().X) / 0.25, (vertex.Y + GetActorLocation().Y) / 0.25);

		if (treeNoise > treeNoiseThreshold)
		{
			int meshNum = FMath::RandRange(0, StaticMeshes.Num() - 1);

			// Set location to vertex position and scale randomly
			FTransform transform;
			transform.SetLocation(vertex + GetActorLocation());
			FQuat Rotation = FVector{ 0,0,0 }.ToOrientationQuat();
			transform.SetRotation(Rotation);
			transform.SetScale3D(FVector{ float(FMath::RandRange(0.8,1.2)) });

			// Add instance of mesh in world with collision disabled
			auto staticMesh = StaticMeshes[meshNum];
			staticMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
			if (staticMesh) { staticMesh->AddInstance(transform); }
		}
	}
}

