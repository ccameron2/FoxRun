// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedPickup.h"

// Sets default values
ASpeedPickup::ASpeedPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Speed Mesh"));
	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void ASpeedPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpeedPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator{ 0,2,0 });
}

