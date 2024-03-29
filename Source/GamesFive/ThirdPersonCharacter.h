// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SpeedPickup.h"
#include "HealthPickup.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "ResourcePickup.h"
#include "ScorePickup.h"
#include "ShieldPickup.h"
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

	void MoveForward(float AxisValue);
	void Strafe(float AxisValue);
	void LookUp(float AxisValue);
	void Jump();
	void SwapCamera();

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

	float MinMovementSpeed;
private:
	UPROPERTY(EditAnywhere)
		float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere)
		float HealthPoints = MaxHealth;

	UPROPERTY(VisibleAnywhere)
		float Score = 0.0f;

	UPROPERTY(VisibleAnywhere)
		float StrafeSpeed = 10.0f;

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		UCameraComponent* ThirdPersonCamera;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ShieldMesh;

	void JumpTimerEnd();
	FTimerHandle JumpTimerHandle;
	void ShieldTimerEnd();
	FTimerHandle ShieldTimerHandle;
	bool CanJump = true;
	bool Shielded = false;

};
