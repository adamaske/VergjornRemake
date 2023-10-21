// Fill out your copyright notice in the Description page of Project Settings.


#include "V_Player.h"

//Components
#include "Camera/CameraComponent.h"
//Movement
#include "GameFramework/FloatingPawnMovement.h"
// Sets default values
AV_Player::AV_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mFloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Movement"));
	

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AV_Player::BeginPlay()
{
	Super::BeginPlay();
	
	//mFloatingPawnMovement->Activate();
	FGuid guid = FGuid::NewGuid();
	FString guidString = guid.ToString();
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("TEST GUID : %s"), *guidString));
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

	// Bind movement events
	InputComponent->BindAxis("MoveForward", this, &AV_Player::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AV_Player::MoveRight);

	InputComponent->BindAxis("Turn", this, &AV_Player::Turn);
	InputComponent->BindAxis("LookUp", this, &AV_Player::LookUp);
}

#pragma region Movement and Rotation
void AV_Player::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * mMoveSpeed * value * GetWorld()->GetDeltaSeconds());
}

void AV_Player::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * mMoveSpeed * value * GetWorld()->GetDeltaSeconds());
}

void AV_Player::Turn(float value)
{
	if (bCanLookAround && value != 0.0f) {
		AddControllerYawInput(value * mTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AV_Player::LookUp(float value)
{
	if (bCanLookAround && value != 0.0f) {
		AddControllerPitchInput(value * mLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AV_Player::EnableRotation()
{
	bCanLookAround = true;
}

void AV_Player::DisableRotation()
{
	bCanLookAround = false;
}

#pragma endregion
