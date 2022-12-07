// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h" 
#include "Materials/MaterialInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourcePickup.generated.h"

UCLASS()
class GAMESFIVE_API AResourcePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourcePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		int Type = 0;

	UPROPERTY(EditAnywhere)
		TArray<UMaterialInterface*> MaterialList;

private:

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ResourceMesh;

	UPROPERTY(EditAnywhere)
		float MagnetIntensity = 0.9f;

	UPROPERTY(EditAnywhere)
		float MagnetRange = 500.0f;

	UPROPERTY()
		FTimerHandle ResourceTimer;

	UFUNCTION()
		void TimeUp();

	UPROPERTY(EditAnywhere)
		float Lifetime = 8.0f;

};
