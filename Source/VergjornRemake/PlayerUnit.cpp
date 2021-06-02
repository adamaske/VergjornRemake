// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerUnit.h"
#include <string>

#include "BuildingManager.h"
#include "Camera/CameraComponent.h"
#include "WorkerUnit.h"
#include "Structure.h"
#include "ResourceTest.h"
#include "GatherableUnit.h"
#include "GatherableHandler.h"
#include "CapacityComponent.h"
// Sets default values
APlayerUnit::APlayerUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
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
	CheckHover();
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
	PlayerInputComponent->BindAction("Select", IE_Pressed, this, &APlayerUnit::Select);
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
}

void APlayerUnit::Select()
{
	//Single press
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (!HitResult.GetActor()) {

		return;
	}

	if (HitResult.GetActor()->IsA(AStructure::StaticClass())) {
		//Select the structure or deselct
		AStructure* structure = Cast<AStructure>(HitResult.GetActor());
		if (SelectedStructure == structure)
		{
			//Do nothing
		}
		//Deselct if already has a structure selected
		if(SelectedStructure)
		{
			SelectedStructure->DeselectedByPlayer();
		}
		//Set new structure
		SelectedStructure = structure;
		SelectedStructure->SelectedByPlayer();
	}
}

void APlayerUnit::Interact() {
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	
	if (!HitResult.GetActor()) {
		return;
	}

	if (HitResult.GetActor()->IsA(AStructure::StaticClass())) 
	{
		AStructure* structure = Cast<AStructure>(HitResult.GetActor());
		for (int i = 0; i < WorkersSelected.Num(); i++)
		{
			structure->GetWorkerUnits(WorkersSelected[i]);
		}
	}else if(HitResult.GetActor()->IsA(AGatherableUnit::StaticClass()))
	{
		//GatherableUnit Refrence
		AGatherableUnit* unit = Cast<AGatherableUnit>(HitResult.GetActor());
		//Set state and gatherable unit
		for (int i = 0; i < WorkersSelected.Num(); i++)
		{
			WorkersSelected[i]->GetGatherableUnit(unit);
		}
	}
	else {
		for (int i = 0; i < WorkersSelected.Num(); i++)
		{
			//Move WorkerUnits to loc
			WorkersSelected[i]->GetDestination(HitResult.Location);
			WorkersSelected[i]->RemoveStructure();
			WorkersSelected[i]->WorkPlace = EWorkPlace::NONE;
		}
		UE_LOG(LogTemp, Log, TEXT("Gave WorkerUnits movedestionation"));
	}
}

void APlayerUnit::MoveWorkerUnits(FVector loc)
{
	UE_LOG(LogTemp, Log, TEXT("Hit structure %s"), *loc.ToString());
	for (int i = 0; i < WorkersSelected.Num(); i++)
	{
		//Move WorkerUnits to loc
		WorkersSelected[i]->GetDestination(loc);
	}
}

void APlayerUnit::CheckHover() {
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (!HitResult.GetActor()) {
		//UE_LOG(LogTemp, Log, TEXT("No actor under HItResult from hover"));
		bIsHovering = false;
		return;
	}
	if (HitResult.GetActor()->IsA(AWorkerUnit::StaticClass())) {
		AWorkerUnit* WorkerUnit = Cast<AWorkerUnit>(HitResult.GetActor());
		HoveredWorker = WorkerUnit;
		bIsHovering = false;
	}
	else if (HitResult.GetActor()->IsA(AStructure::StaticClass())) {
		AStructure* structure = Cast<AStructure>(HitResult.GetActor());
		//Display structure information
		//bIsHovering = false;
	}
	else {
		bIsHovering = false;
	}
	
}
bool APlayerUnit::GetResources(ResourceType newType, float amount)
{
	UE_LOG(LogTemp, Log, TEXT("Got resources"));
	for (int i = 0; i < ResourceAmounts.Num(); i++)
	{
		if(ResourceAmounts[i].myType == newType)
		{
			float TotalCapacity = ResourceAmounts[i].BaseCapacity + ResourceAmounts[i].ExtraCapacity;
			if(ResourceAmounts[i].Amount + amount < TotalCapacity)
			{
				ResourceAmounts[i].Amount += amount;
			}
		}
	}
	return true;
}

void APlayerUnit::GetBuildingManager(ABuildingManager* manager)
{
	BuildingManager = manager;
}