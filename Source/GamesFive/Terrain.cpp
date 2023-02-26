// Fill out your copyright notice in the Description page of Project Settings.
#include "Terrain.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
ATerrain::ATerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TerrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Terrain Mesh"));
	SetRootComponent(TerrainMesh);

	ConstructorHelpers::FObjectFinder<UMaterialInstance> grassMaterial(TEXT("M_Terrain_Grass'/Game/Materials/M_Terrain_Grass.M_Terrain_Grass'"));
	GrassMaterial = grassMaterial.Object;
	ConstructorHelpers::FObjectFinder<UMaterialInstance> deepGrassMaterial(TEXT("M_Terrain_DeepGrass'/Game/Materials/M_Terrain_DeepGrass.M_Terrain_DeepGrass'"));
	DeepGrassMaterial = deepGrassMaterial.Object;
	ConstructorHelpers::FObjectFinder<UMaterialInstance> gravelMaterial(TEXT("M_Terrain_Gravel'/Game/Materials/M_Terrain_Gravel.M_Terrain_Gravel'"));
	GravelMaterial = gravelMaterial.Object;

	LoadTreeModels();
	LoadObstacleModels();
}

void ATerrain::CreateLanes(FastNoise* noise)
{
	for (int i = 0; i < NumLanes; i++)
	{
		GeometryData laneGeo;

		// Assign lane types
		laneGeo.Lane = TEnumAsByte<LaneType>(i);
		if (i == 4) { laneGeo.Lane = TEnumAsByte<LaneType>(0); }

		laneGeo.Vertices.Init({ 0,0,0 }, SizeX * SizeY);

		// Create a grid of vertices
		int indexX = 0;
		for (int j = 0; j < SizeX; j++)
		{
			int indexY = 0;
			for (int k = 0; k < SizeY; k++)
			{
				FVector newVector = FVector{ float(j * Scale), float(k * Scale),0 };
				laneGeo.Vertices[SizeY * indexX + indexY] = newVector;
				indexY++;
			}
			indexX++;
		}
		// Generate triangles for grid of vertices
		UKismetProceduralMeshLibrary::CreateGridMeshTriangles(SizeX, SizeY, false, laneGeo.Triangles);

		for(auto& vertex : laneGeo.Vertices)
		{
			vertex.X += (LaneXOffset * Scale * i) + (i * Scale);
		}

		// Store the tallest vector index and height
		float tallestVectorHeight = 0;
		int tallestVector = 0;

		// For each vertex, get 2 different noise values and apply them to vertex height at different scales.
		for (int j = 0; j < laneGeo.Vertices.Num(); j++)
		{
			// Get input vector from vertex list and sample noise at different levels
			auto input = laneGeo.Vertices[j];
			auto result1 = noise->GetNoise((input.X + GetActorLocation().X) / 300, (input.Y + GetActorLocation().Y) / 300);
			laneGeo.Vertices[j].Z += result1 * 200;
			auto result2 = noise->GetNoise((input.X + GetActorLocation().X) / 20, (input.Y + GetActorLocation().Y) / 20);
			laneGeo.Vertices[j].Z += result2 * 100;

			// Find the tallest vector and store in variables
			if (laneGeo.Vertices[j].Z > tallestVectorHeight)
			{
				tallestVector = j;
				tallestVectorHeight = laneGeo.Vertices[j].Z;
			}
		}

		laneGeo.Normals = CalculateNormals(laneGeo.Vertices, laneGeo.Triangles);

		TerrainMesh->CreateMeshSection(i, laneGeo.Vertices, laneGeo.Triangles, laneGeo.Normals, laneGeo.UVs, laneGeo.VertexColours, laneGeo.Tangents, true);

		if(laneGeo.Lane == Valley || laneGeo.Lane == Deep)	TerrainMesh->SetMaterial(i, DeepGrassMaterial);
		else if (laneGeo.Lane == Gravel) TerrainMesh->SetMaterial(i, GravelMaterial);
		else if(laneGeo.Lane == Grass) TerrainMesh->SetMaterial(i, GrassMaterial);
		LaneGeometry.Push(laneGeo);
		PlaceObstacles(noise,i);
	}
	PlaceTrees(noise);
}

void ATerrain::Destroyed()
{
	Super::Destroyed();

	for (auto& obstacle : GrassObstacles)
	{
		obstacle->Destroy();
	}
	for (auto& obstacle : RoadObstacles)
	{
		obstacle->Destroy();
	}
	for (auto& obstacle : WaterObstacles)
	{
		obstacle->Destroy();
	}
}

// Called when the game starts or when spawned
void ATerrain::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FVector> ATerrain::CalculateNormals(TArray<FVector> vertices, TArray<int32> triangles)
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

void ATerrain::PlaceTrees(FastNoise* noise)
{
	GeometryData valleys[2];
	valleys[0] = LaneGeometry[0];
	valleys[1] = LaneGeometry[LaneGeometry.Num() - 1];

	for(auto& valley : valleys)
	{
		for(auto& vertex : valley.Vertices)
		{
			float treeNoise = noise->GetNoise((vertex.X + GetActorLocation().X) / 0.25, (vertex.Y + GetActorLocation().Y) / 0.25);

			if(treeNoise > treeNoiseThreshold)
			{
				int meshNum = FMath::RandRange(0, ValleyStaticMeshes.Num() - 1);

				// Set location to vertex position and scale randomly
				FTransform transform;
				transform.SetLocation(vertex + GetActorLocation());
				FQuat Rotation = FVector{ 0,0,0 }.ToOrientationQuat();
				transform.SetRotation(Rotation);
				transform.SetScale3D(FVector{ float(FMath::RandRange(0.8,1.2)) });

				// Add instance of mesh in world with collision disabled
				auto staticMesh = ValleyStaticMeshes[meshNum];
				staticMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
				if (staticMesh) { staticMesh->AddInstance(transform); }
			}
		}
	}
}

void ATerrain::PlaceObstacles(FastNoise* noise, int lane)
{
	if (lane == 0 || lane == 4) { return; }

	auto meshes = GrassMeshes;

	for (int i = 0; i < LaneGeometry[lane].Vertices.Num(); i += 8)
	{
		auto vertex = LaneGeometry[lane].Vertices[i];
		if (vertex.X == i * SizeX * Scale || vertex.X == i - 1 * SizeX * Scale) continue;
		float obstacleNoise = noise->GetNoise((vertex.X + GetActorLocation().X), (vertex.Y + GetActorLocation().Y));
		if (obstacleNoise > obstacleNoiseThreshold)
		{
			int meshNum = FMath::RandRange(0, meshes.Num() - 1);

			// Set location to vertex position and scale randomly
			FTransform transform;
			transform.SetLocation(vertex + GetActorLocation());
			FQuat rotation = FVector{ 0,0,0 }.ToOrientationQuat();		
			FVector scale = FVector{ float(FMath::RandRange(0.8, 1.2)) };

			auto staticMesh = meshes[meshNum];
			if (meshNum == 1 || meshNum == 2) scale = { 2,2,2 };
			if (meshNum == 4) scale = { 2.5f,2.5f,2.5f };
			transform.SetScale3D(scale);
			transform.SetRotation(rotation);
			auto newObstacle = GetWorld()->SpawnActor<AObstacle>(ObstacleClass, transform);
			newObstacle->StaticMesh->SetStaticMesh(staticMesh);
			GrassObstacles.Push(newObstacle);
		}
	}
}

void ATerrain::LoadObstacleModels()
{
	// Load asset and store in object
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset1(TEXT("StaticMesh'/Game/Models/Nature/Bush_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset2(TEXT("StaticMesh'/Game/Models/Nature/Rock_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset3(TEXT("StaticMesh'/Game/Models/Nature/Rock_Moss_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset4(TEXT("StaticMesh'/Game/Models/Nature/TreeStump'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset5(TEXT("StaticMesh'/Game/Models/Crops/Mushroom_3'"));

	// Create new instanced SMC and push into vector
	GrassMeshes.Push(MeshAsset1.Object);
	GrassMeshes.Push(MeshAsset2.Object);
	GrassMeshes.Push(MeshAsset3.Object);
	GrassMeshes.Push(MeshAsset4.Object);
	GrassMeshes.Push(MeshAsset5.Object);
}

void ATerrain::LoadTreeModels()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset1(TEXT("StaticMesh'/Game/Models/Nature/CommonTree_2'"));
	ValleyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree1 Static Mesh")));
	ValleyStaticMeshes[0]->SetStaticMesh(MeshAsset1.Object);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset2(TEXT("StaticMesh'/Game/Models/Nature/PineTree_5'"));
	ValleyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree2 Static Mesh")));
	ValleyStaticMeshes[1]->SetStaticMesh(MeshAsset2.Object);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset3(TEXT("StaticMesh'/Game/Models/Nature/CommonTree_5'"));
	ValleyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree3 Static Mesh")));
	ValleyStaticMeshes[2]->SetStaticMesh(MeshAsset3.Object);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset4(TEXT("StaticMesh'/Game/Models/Nature/CommonTree_4'"));
	ValleyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree4 Static Mesh")));
	ValleyStaticMeshes[3]->SetStaticMesh(MeshAsset4.Object);
}
