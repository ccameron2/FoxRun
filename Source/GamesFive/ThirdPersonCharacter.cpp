// Fill out your copyright notice in the Description page of Project Settings.
#include "ThirdPersonCharacter.h"
#include "GameFramework/PawnMovementComponent.h" 

// Sets default values
AThirdPersonCharacter::AThirdPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create spring arm and attach to the root component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));

	// Spring Arm settings
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->SetRelativeRotation((new FRotator(0.0f, 0.0f, 0.0f))->Quaternion());
	SpringArm->SetRelativeLocation(FVector(-361.0f, -305.0f, 113.0f));
	SpringArm->SetupAttachment(RootComponent);

	//Create Camera and attach to SpringArm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AThirdPersonCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AThirdPersonCharacter::OnOverlapEnd);

	//Possess player 0
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Energy < MaxEnergy){ Energy++; }
	if (ExpPoints >= MaxExp) { LevelUp(); }
	
}

// Called to bind functionality to input
void AThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AThirdPersonCharacter::MoveForward(float AxisValue)
{
	//Move character forwards
	if (Controller != nullptr && AxisValue != 0)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, AxisValue);
	}
	//AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AThirdPersonCharacter::Strafe(float AxisValue)
{
	//Strafe character left and right
	if (Controller != nullptr && AxisValue != 0)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, AxisValue);
	}
	//AddMovementInput(GetActorRightVector() * AxisValue);
}

void AThirdPersonCharacter::Turn(float AxisValue)
{
	//Turn character left and right
	AddControllerYawInput(AxisValue);
}

void AThirdPersonCharacter::LookUp(float AxisValue)
{
	//Rotate view up and down
	AddControllerPitchInput(AxisValue);
}

void AThirdPersonCharacter::Fire()
{
}

void AThirdPersonCharacter::Jump()
{
	Super::Jump();
	auto MovementComponent = GetMovementComponent();

	if (Energy > 50 && !MovementComponent->IsFalling())
	{
		Energy -= 75;
	}	
}

float AThirdPersonCharacter::GetMaxHealth()
{
	return MaxHealth;
}

float AThirdPersonCharacter::GetMaxEnergy()
{
	return MaxEnergy;
}

float AThirdPersonCharacter::GetMaxMana()
{
	return MaxMana;
}

float AThirdPersonCharacter::GetMaxGold()
{
	return MaxGold;
}

float AThirdPersonCharacter::GetMaxExperience()
{
	return MaxExp;
}

float AThirdPersonCharacter::GetHealth()
{
	return HealthPoints;
}

float AThirdPersonCharacter::GetEnergy()
{
	return Energy;
}

float AThirdPersonCharacter::GetMana()
{
	return Mana;
}

float AThirdPersonCharacter::GetGold()
{
	return Gold;
}

float AThirdPersonCharacter::GetExperience()
{
	return ExpPoints;
}

float AThirdPersonCharacter::GetLives()
{
	return Lives;
}

float AThirdPersonCharacter::GetPlayerLevel()
{
	return Level;
}

void AThirdPersonCharacter::LevelUp()
{
	Level++; 
	ExpPoints = 0.0f; 
	MaxExp += 5;
	MaxHealth += 5;
	MaxMana += 5;
	MaxEnergy += 5;
}

float AThirdPersonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Take Damage"));
	HealthPoints -= DamageAmount;
	return 0.0f;
}

void AThirdPersonCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
		AResourcePickup* resourcePickup = Cast<AResourcePickup>(OtherActor);
		if (resourcePickup)
		{
			if (resourcePickup->Type == 0) { if(HealthPoints < MaxHealth ){ HealthPoints++; }}
			if (resourcePickup->Type == 1) { if(Mana		 < MaxMana	 ){ Mana++; }}
			if (resourcePickup->Type == 2) { if(Energy		 < MaxEnergy ){ Energy++; }}
			if (resourcePickup->Type == 3) { if(ExpPoints	 < MaxExp	 ){ ExpPoints++; }}
			if (resourcePickup->Type == 4) { if(Gold		 < MaxGold	 ){ Gold++; }}

		}
		OtherActor->Destroy();
	}
}

void AThirdPersonCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));
	}
}