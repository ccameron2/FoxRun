// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "ResourcePickup.h"
#include "Obstacle.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCharacter.generated.h"

UCLASS()
class GAMESFIVE_API AThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThirdPersonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void MoveForward(float AxisValue);
	void Strafe(float AxisValue);
	void Turn(float AxisValue);
	void LookUp(float AxisValue);
	void Fire();
	void Jump();
	void SwapCamera();
	void ToggleSprint();

	bool Walking = false;

	UPROPERTY(EditAnywhere)
		USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere)
		float RotationSpeed = 500.0f;

	UFUNCTION(BlueprintCallable)
		float GetMaxHealth() { return MaxHealth; }

	UFUNCTION(BlueprintCallable)
		float GetHealth() { return HealthPoints; }

	UFUNCTION(BlueprintCallable)
		float GetScore() { return Score; }

	UFUNCTION()
		void AddScore(int amount);

private:
	UPROPERTY(EditAnywhere)
		float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere)
		float HealthPoints = MaxHealth;

	UPROPERTY(VisibleAnywhere)
		float Score = 0.0f;

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
		UCameraComponent* ThirdPersonCamera;

	UPROPERTY(EditAnywhere)
		UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm;

	void JumpTimerEnd();
	FTimerHandle JumpTimerHandle;
	bool CanJump = true;

};
