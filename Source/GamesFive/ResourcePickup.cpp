// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourcePickup.h"
#include "Kismet/KismetMathLibrary.h" 

// Sets default values
AResourcePickup::AResourcePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Health Mesh"));
	ResourceMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AResourcePickup::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(ResourceTimer, this, &AResourcePickup::TimeUp, Lifetime, false);
	ResourceMesh->SetMaterial(0, MaterialList[Type]);
}

// Called every frame
void AResourcePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Location = GetActorLocation();
	AActor* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	FVector PlayerLocation = PlayerPawn->GetActorLocation() + FVector{0,0,300};
	float DistanceToTravel = FVector::Distance(PlayerLocation, Location);
	if (DistanceToTravel < MagnetRange)
	{
		FVector UnitDirectionVector = UKismetMathLibrary::GetDirectionUnitVector(Location, PlayerLocation);
		ResourceMesh->AddImpulse(UnitDirectionVector * ((MagnetIntensity * DistanceToTravel) / 40));
	}
	if (!ResourceTimer.IsValid())
	{
		auto newLoc = FMath::VInterpTo(GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), DeltaTime, 5.0f);
		SetActorLocation(newLoc);
	}
}

void AResourcePickup::TimeUp()
{
	ResourceTimer.Invalidate();
}



