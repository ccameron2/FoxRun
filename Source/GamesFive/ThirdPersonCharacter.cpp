// Fill out your copyright notice in the Description page of Project Settings.
#include "ThirdPersonCharacter.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/GameplayStatics.h"
// Sets default values
AThirdPersonCharacter::AThirdPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetupAttachment(RootComponent);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->InitSphereRadius(0.8f);
	CollisionSphere->SetupAttachment(GetMesh(), "head");

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield Mesh"));
	ShieldMesh->SetupAttachment(GetMesh());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));

	// Spring Arm settings
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	SpringArm->SetRelativeLocation(FVector(-361.0f, -305.0f, 113.0f));
	SpringArm->SetupAttachment(RootComponent);

	// Create third person camera and attach to SpringArm
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Third Person Camera"));
	ThirdPersonCamera->SetupAttachment(SpringArm);

	//Possess player 0
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AThirdPersonCharacter::OnOverlapBegin);
	ThirdPersonCamera->SetActive(true);
	MinMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;
	ShieldMesh->ToggleVisibility();
}

// Called every frame
void AThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveForward(1);
	if (HealthPoints <= 0)
	{
		Destroy();
	}
	ShieldMesh->AddLocalRotation(FRotator{ 0,2,0 });
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

		if(!Walking) AddMovementInput(Direction, AxisValue);
		else AddMovementInput(Direction, AxisValue / 4);
	}
}

void AThirdPersonCharacter::Strafe(float AxisValue)
{
	//Strafe character left and right
	if (Controller != nullptr && AxisValue != 0)
	{
		if (GetCharacterMovement()->IsFalling()) return;
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		SetActorLocation(GetActorLocation() += GetActorRightVector() * StrafeSpeed * AxisValue);
	}
}

void AThirdPersonCharacter::LookUp(float AxisValue)
{
	//Rotate view up and down
	AddControllerPitchInput(AxisValue);
}

void AThirdPersonCharacter::Jump()
{
	if (!CanJump) return;
	Super::Jump();
	CanJump = false;
	GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, this, &AThirdPersonCharacter::JumpTimerEnd, 1.2f);
}

void AThirdPersonCharacter::SwapCamera()
{
	ThirdPersonCamera->ToggleActive();
}

void AThirdPersonCharacter::AddScore(int amount)
{
	Score += amount;
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
		auto obstacle = Cast<AObstacle>(OtherActor);
		if (obstacle)
		{
			if(!Shielded)	HealthPoints -= 20;
		}
		auto health = Cast<AHealthPickup>(OtherActor);
		if (health)
		{
			if (HealthPoints + health->HealthAmount > MaxHealth) HealthPoints = MaxHealth;
			else HealthPoints += health->HealthAmount;
		}
		auto score = Cast<AScorePickup>(OtherActor);
		if (score)
		{
			Score += score->ScoreAmount;
		}
		auto speed = Cast<ASpeedPickup>(OtherActor);
		if (speed)
		{
			if (GetCharacterMovement()->MaxWalkSpeed - speed->SpeedAmount < MinMovementSpeed) GetCharacterMovement()->MaxWalkSpeed = MinMovementSpeed;
			else GetCharacterMovement()->MaxWalkSpeed -= speed->SpeedAmount;
		}
		auto shield = Cast<AShieldPickup>(OtherActor);
		if (shield)
		{
			if(Shielded) return;
			ShieldMesh->ToggleVisibility();
			Shielded = true;
			GetWorld()->GetTimerManager().SetTimer(ShieldTimerHandle, this, &AThirdPersonCharacter::ShieldTimerEnd, shield->ShieldDuration);
		}
		OtherActor->Destroy();
	}
}

void AThirdPersonCharacter::JumpTimerEnd()
{
	CanJump = true;
}

void AThirdPersonCharacter::ShieldTimerEnd()
{
	ShieldMesh->ToggleVisibility();
	Shielded = false;
}
