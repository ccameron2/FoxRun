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
	InputComponent->BindAxis("Turn", this, &AThirdPersonPlayerController::CallTurn);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AThirdPersonPlayerController::CallFire);
	InputComponent->BindAxis("Strafe", this, &AThirdPersonPlayerController::CallStrafe);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AThirdPersonPlayerController::CallJump);
	InputComponent->BindAxis("Look Up", this, &AThirdPersonPlayerController::CallLookUp);
	//InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AThirdPersonPlayerController::CallSprint);

}

void AThirdPersonPlayerController::CallForward(float Value)
{
	if (PlayerPawn)
	{
		PlayerPawn->MoveForward(Value);
	}
}

void AThirdPersonPlayerController::CallTurn(float Value)
{
	if (PlayerPawn)
	{
		PlayerPawn->Turn(Value);
	}
}


void AThirdPersonPlayerController::CallFire()
{
	//if (TimesShot < MaximumAmmo)
	//{
		if (PlayerPawn)
		{
			//TimesShot++;
			//ShotsLeft = MaximumAmmo - TimesShot;
			PlayerPawn->Fire();
		}
	//}

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

void AThirdPersonPlayerController::CallLookUp(float Value)
{
	if (PlayerPawn)
	{
		PlayerPawn->LookUp(Value);
	}
}
