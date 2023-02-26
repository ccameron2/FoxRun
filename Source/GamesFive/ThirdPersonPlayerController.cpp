// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonPlayerController.h"

void AThirdPersonPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//Get pawn reference
	PlayerPawn = Cast<AThirdPersonCharacter>(GetPawn());
}

void AThirdPersonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	//Bind inputs to assigned functions
	InputComponent->BindAxis("Move Forwards", this, &AThirdPersonPlayerController::CallForward);
	InputComponent->BindAxis("Strafe", this, &AThirdPersonPlayerController::CallStrafe);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AThirdPersonPlayerController::CallJump);
}

void AThirdPersonPlayerController::CallForward(float Value)
{
	if (PlayerPawn)
	{
		PlayerPawn->MoveForward(Value);
	}
}

void AThirdPersonPlayerController::CallStrafe(float Value)
{
	if (PlayerPawn)
	{
		PlayerPawn->Strafe(Value);
	}
}

void AThirdPersonPlayerController::CallJump()
{
	if (PlayerPawn)
	{
		PlayerPawn->Jump();
	}
}