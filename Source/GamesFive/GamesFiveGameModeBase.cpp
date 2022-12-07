// Copyright Epic Games, Inc. All Rights Reserved.


#include "GamesFiveGameModeBase.h"

void AGamesFiveGameModeBase::StartPlay()
{
	Super::StartPlay();
	FActorSpawnParameters SpawnParams;
	FVector Location;
	FTransform Transform;
	FVector Scale = FVector(0.08, 0.08, 0.08);
	for (int i = 0; i < 1000; i++)
	{		
		Location.X = FMath::RandRange(0, 500);
		Location.Y = FMath::RandRange(0, 500);
		Location.Z = FMath::RandRange(0, 500);
		Transform.SetScale3D(Scale);
		Transform.SetTranslation(Location);
		AResourcePickup* ResourcePickup = GetWorld()->SpawnActorDeferred<AResourcePickup>(HealthClass, Transform);
		ResourcePickup->Type = FMath::RandRange(0, ResourcePickup->MaterialList.Num() - 1);
		ResourcePickup->FinishSpawning(Transform);
	}
}
