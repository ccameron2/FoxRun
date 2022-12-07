// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "ResourcePickup.h"

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
	void Turn(float AxisValue);
	void LookUp(float AxisValue);
	void Fire();
	void Jump();

	UPROPERTY(EditAnywhere)
		float MoveSpeed = 200.0f;

	UPROPERTY(EditAnywhere)
		float RotationSpeed = 500.0f;

	UPROPERTY(EditAnywhere)
		float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere)
		float MaxEnergy = 100.0f;

	UPROPERTY(EditAnywhere)
		float MaxExp = 100.0f;

	UPROPERTY(EditAnywhere)
		float MaxMana = 100.0f;

	UPROPERTY(EditAnywhere)
		float MaxGold = 100.0f;

	UFUNCTION(BlueprintCallable)
		float GetHealth();

	UFUNCTION(BlueprintCallable)
		float GetEnergy();

	UFUNCTION(BlueprintCallable)
		float GetMana();

	UFUNCTION(BlueprintCallable)
		float GetGold();

	UFUNCTION(BlueprintCallable)
		float GetExperience();

	UFUNCTION(BlueprintCallable)
		float GetLives();

	UFUNCTION(BlueprintCallable)
		float GetPlayerLevel();

	UFUNCTION(BlueprintCallable)
		void LevelUp();

	UFUNCTION(BlueprintCallable)
		float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
		float GetMaxEnergy();

	UFUNCTION(BlueprintCallable)
		float GetMaxMana();

	UFUNCTION(BlueprintCallable)
		float GetMaxGold();

	UFUNCTION(BlueprintCallable)
		float GetMaxExperience();

private:

	UPROPERTY(VisibleAnywhere)
		float HealthPoints = MaxHealth;

	UPROPERTY(VisibleAnywhere)
		float ExpPoints = 0.0f;

	UPROPERTY(VisibleAnywhere)
		float Energy = MaxEnergy;

	UPROPERTY(VisibleAnywhere)
		float Gold = 0.0f;

	UPROPERTY(VisibleAnywhere)
		float Mana = MaxMana;

	UPROPERTY(VisibleAnywhere)
		int Level = 1;

	UPROPERTY(VisibleAnywhere)
		int Lives = 1;

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm;

};
