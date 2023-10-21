// Fill out your copyright notice in the Description page of Project Settings.


#include "V_Player.h"

//Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

//Components
#include "Camera/CameraComponent.h"
//Movement
#include "GameFramework/FloatingPawnMovement.h"
// Sets default values
AV_Player::AV_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floasdting Pawn Movement"));


	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Camera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AV_Player::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(m_DefaultMappingContext, 0);

		}
	}
}

// Called every frame
void AV_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AV_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(m_MoveAction, ETriggerEvent::Triggered, this, &AV_Player::Move);
		EnhancedInputComponent->BindAction(m_LookAction, ETriggerEvent::Completed, this, &AV_Player::Look);

	}
}

void AV_Player::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	auto Forward = GetActorForwardVector();
	auto Right = GetActorRightVector();

	auto MoveSpeed = m_MoveSpeed * GetWorld()->GetDeltaSeconds();

	AddMovementInput(Forward, MoveSpeed);
	AddMovementInput(Right, MoveSpeed);

}

void AV_Player::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (!Controller) {
		return;
	}

	auto DeltaSeconds = GetWorld()->GetDeltaSeconds();

	float X = LookAxisVector.X * m_TurnRate * DeltaSeconds;
	float Y = LookAxisVector.Y * m_LookUpRate * DeltaSeconds;

	AddControllerYawInput(X);
	AddControllerPitchInput(Y);
}

