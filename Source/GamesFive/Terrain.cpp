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

		// Shift vertices into correct position
		for(auto& vertex : laneGeo.Vertices)
		{
			vertex.X += (LaneXOffset * Scale * i) + (i * Scale);
		}

		// For each vertex, get 2 different noise values and apply them to vertex height at different scales.
		for (int j = 0; j < laneGeo.Vertices.Num(); j++)
		{
			// Get input vector from vertex list and sample noise at different levels
			auto input = laneGeo.Vertices[j];
			auto result1 = noise->GetNoise((input.X + GetActorLocation().X) / 300, (input.Y + GetActorLocation().Y) / 300);
			laneGeo.Vertices[j].Z += result1 * 100;
			auto result2 = noise->GetNoise((input.X + GetActorLocation().X) / 20, (input.Y + GetActorLocation().Y) / 20);
			laneGeo.Vertices[j].Z += result2 * 70;
		}

		laneGeo.Normals = CalculateNormals(laneGeo.Vertices, laneGeo.Triangles);

		TerrainMesh->CreateMeshSection(i, laneGeo.Vertices, laneGeo.Triangles, laneGeo.Normals, laneGeo.UVs, laneGeo.VertexColours, laneGeo.Tangents, true);

		// Set material per lane
		if(laneGeo.Lane == Valley || laneGeo.Lane == Deep || laneGeo.Lane == Grass)	TerrainMesh->SetMaterial(i, DeepGrassMaterial);
		else if (laneGeo.Lane == Gravel) TerrainMesh->SetMaterial(i, GravelMaterial);
		
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

	for (auto& pickup : HealthPickups)
	{
		pickup->Destroy();
	}
	for (auto& pickup : ScorePickups)
	{
		pickup->Destroy();
	}
	for (auto& pickup : ShieldPickups)
	{
		pickup->Destroy();
	}
	for (auto& pickup : SpeedPickups)
	{
		pickup->Destroy();
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
			// Sample noise to determine tree placement
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

	for (int i = 0; i < LaneGeometry[lane].Vertices.Num(); i += 6)
	{
		auto vertex = LaneGeometry[lane].Vertices[i];

		// Vertices on edges of lanes are shared so get sampled twice. Skip these
		if (vertex.X == i * SizeX * Scale || vertex.X == i - 1 * SizeX * Scale) continue;

		// Sample noise to determine obstacle placement
		float obstacleNoise = noise->GetNoise((vertex.X + GetActorLocation().X) / 0.5, (vertex.Y + GetActorLocation().Y) / 0.5);

		if (obstacleNoise > obstacleNoiseThreshold)
		{
			int meshNum = FMath::RandRange(0, meshes.Num() - 1);

			// Set location to vertex position and scale randomly
			FTransform transform;
			FVector location = vertex + GetActorLocation();
			transform.SetLocation(location);
			FQuat rotation = FVector{ 0,0,0 }.ToOrientationQuat();		
			FVector scale = FVector{ float(FMath::RandRange(0.8, 1.2)) };

			// Roll for a mesh to use
			auto staticMesh = meshes[meshNum];
			if (meshNum == 8 || meshNum == 9 || meshNum == 6)
			{
				meshNum = FMath::RandRange(0, meshes.Num() - 1); // Roll again if pickup to reduce frequency
				staticMesh = meshes[meshNum];
			}

			// Scale according to mesh
			if (meshNum == 0 || meshNum == 1) scale = FVector{ float(FMath::RandRange(0.6f, 1.1f)) };
			else if (meshNum == 2 || meshNum == 3) scale = FVector{ float(FMath::RandRange(1.3f, 2.5f)) };
			else if (meshNum == 4) scale = FVector{ float(FMath::RandRange(1.0f, 1.2f)) };
			else if (meshNum == 5) scale = FVector{ float(FMath::RandRange(2.5f, 3.0f)) };
			else if (meshNum == 6) scale = FVector{ float(0.1f) };
			else if (meshNum == 7) scale = FVector{ float(0.1f) };
			else if (meshNum == 9) scale = FVector{ float(0.3f) };
			else if (meshNum == 8) scale = FVector{ float(0.05f)};
			transform.SetScale3D(scale);
			transform.SetRotation(rotation);

			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

			// Spawn health pickup
			if (meshNum == 6)
			{
				location.Z += FMath::RandRange(2, 5) * Scale;
				transform.SetLocation(location);
				auto newPickup = GetWorld()->SpawnActor<AHealthPickup>(HealthClass, transform, params);
				newPickup->StaticMesh->SetStaticMesh(staticMesh);
				HealthPickups.Push(newPickup);
			}
			// Spawn score pickup
			else if (meshNum == 7)
			{
				location.Z += FMath::RandRange(2, 5) * Scale;
				
				for (int j = 0; j < FMath::RandRange(3, 5); j++)
				{
					transform.SetLocation(location);
					auto newPickup = GetWorld()->SpawnActor<AScorePickup>(ScoreClass, transform, params);
					newPickup->StaticMesh->SetStaticMesh(staticMesh);
					ScorePickups.Push(newPickup);
					location.Y += 2 * Scale;
				}
			}
			// Spawn speed pickup
			else if(meshNum == 8)
			{
				location.Z += FMath::RandRange(2, 5) * Scale;
				rotation = FVector{ -90,0,0 }.ToOrientationQuat();
				transform.SetRotation(rotation);
				transform.SetLocation(location);
				auto newPickup = GetWorld()->SpawnActor<ASpeedPickup>(SpeedClass, transform, params);
				newPickup->StaticMesh->SetStaticMesh(staticMesh);
				SpeedPickups.Push(newPickup);
			}
			// Spawn shield pickup
			else if (meshNum == 9)
			{
				location.Z += FMath::RandRange(2, 5) * Scale;
				rotation = FVector{ -90,0,0 }.ToOrientationQuat();
				transform.SetRotation(rotation);
				transform.SetLocation(location);
				auto newPickup = GetWorld()->SpawnActor<AShieldPickup>(ShieldClass, transform, params);
				newPickup->StaticMesh->SetStaticMesh(staticMesh);
				ShieldPickups.Push(newPickup);
			}
			// Spawn obstacle
			else
			{
				auto newObstacle = GetWorld()->SpawnActor<AObstacle>(ObstacleClass, transform, params);
				newObstacle->StaticMesh->SetStaticMesh(staticMesh);
				GrassObstacles.Push(newObstacle);
			}			
		}
	}
}

void ATerrain::LoadObstacleModels()
{
	// Load asset and store in object
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset1(TEXT("StaticMesh'/Game/Models/Nature/Bush_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset2(TEXT("StaticMesh'/Game/Models/Nature/BushBerries_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset3(TEXT("StaticMesh'/Game/Models/Nature/Rock_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset4(TEXT("StaticMesh'/Game/Models/Nature/Rock_Moss_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset5(TEXT("StaticMesh'/Game/Models/Nature/TreeStump'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset6(TEXT("StaticMesh'/Game/Models/Crops/Mushroom_3'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset7(TEXT("StaticMesh'/Game/Models/Cross'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset8(TEXT("StaticMesh'/Game/Models/Icosahedron'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset9(TEXT("StaticMesh'/Game/Models/Arrow'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset10(TEXT("StaticMesh'/Game/Models/Shield'"));

	// Create new instanced SMC and push into vector
	GrassMeshes.Push(MeshAsset1.Object);
	GrassMeshes.Push(MeshAsset2.Object);
	GrassMeshes.Push(MeshAsset3.Object);
	GrassMeshes.Push(MeshAsset4.Object);
	GrassMeshes.Push(MeshAsset5.Object);
	GrassMeshes.Push(MeshAsset6.Object);
	GrassMeshes.Push(MeshAsset7.Object);
	GrassMeshes.Push(MeshAsset8.Object);
	GrassMeshes.Push(MeshAsset9.Object);
	GrassMeshes.Push(MeshAsset10.Object);
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
