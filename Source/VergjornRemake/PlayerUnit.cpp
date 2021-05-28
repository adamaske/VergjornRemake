// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUnit.h"
#include "Camera/CameraComponent.h"
#include "Worker.h"
#include "Structure.h"
// Sets default values
APlayerUnit::APlayerUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerUnit::BeginPlay()
{
	Super::BeginPlay();
	
	PC = Cast<APlayerController>(GetController());
	PC->bShowMouseCursor = true;
}

// Called every frame
void APlayerUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (!MovementVector.IsNearlyZero()) {
		FVector newLocation = GetActorLocation() + (MovementVector * CameraMovementSpeed * DeltaTime);

		SetActorLocation(newLocation);
	}
}


// Called to bind functionality to input
void APlayerUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("StartSelection", IE_Pressed, this, &APlayerUnit::StartSelection);
	PlayerInputComponent->BindAction("StartSelection", IE_Released, this, &APlayerUnit::EndSelection);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerUnit::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerUnit::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &APlayerUnit::MoveUp);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerUnit::Interact);
}

void APlayerUnit::MoveForward(float value)
{
	MovementVector.X = value;
}

void APlayerUnit::MoveRight(float value)
{
	MovementVector.Y = value;
}

void APlayerUnit::MoveUp(float value)
{
	MovementVector.Z = value;
}

void APlayerUnit::StartSelection() {
	OnStartSelection();
	WorkersSelected.Empty();
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (!HitResult.GetActor()) {
		return;
	}
	
	StartSelectionLocation = HitResult.Location;
}

void APlayerUnit::EndSelection() {
	OnEndSelection();
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (!HitResult.GetActor()) {
		return;
	}

	EndSelectionLocation = HitResult.Location;
	OnEndSelection();
	//Check for worker or structure
	/*
	if (HitResult.GetActor()->IsA(AWorker::StaticClass())) {
		AWorker* worker = Cast<AWorker>(HitResult.GetActor());
		if (!WorkersSelected.Contains(worker)) {
			WorkersSelected.Add(worker);
			UE_LOG(LogTemp, Log, TEXT("Added a worker to selection"))
			return;
		}
	}
	*/
}

void APlayerUnit::Interact() {
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (!HitResult.GetActor()) {
		return;
	}

	if (HitResult.GetActor()->IsA(AStructure::StaticClass())) {
		AStructure* structure = Cast<AStructure>(HitResult.GetActor());
		UE_LOG(LogTemp, Log, TEXT("Hit structure"));
		if (WorkersSelected.Num() > 0) {
			structure->GetWorkers(WorkersSelected);
			WorkersSelected.Empty();
		}
	}
	else {
		if (WorkersSelected.Num() > 0) {
			MoveWorkers(HitResult.Location);
		}
	}
}

void APlayerUnit::MoveWorkers(FVector loc)
{
	for (int i = 0; i < WorkersSelected.Num(); i++)
	{
		//Move workers to loc
	}
}
