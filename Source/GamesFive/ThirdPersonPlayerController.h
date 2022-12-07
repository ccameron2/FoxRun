// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ThirdPersonCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThirdPersonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAMESFIVE_API AThirdPersonPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	virtual void SetupInputComponent();

	UPROPERTY()
		class AThirdPersonCharacter* PlayerPawn;

private:

	virtual void CallForward(float Value);
	virtual void CallTurn(float Value);
	virtual void CallFire();
	virtual void CallStrafe(float Value);
	virtual void CallJump();
	virtual void CallLookUp(float Value);
	

};
