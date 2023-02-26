// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkerUnit.h"
#include "Structure.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GatherableUnit.h"
// Sets default values
AWorkerUnit::AWorkerUnit()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	WorkerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMainMesh"));
	WorkerMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AWorkerUnit::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
	ChangeMesh();
}

// Called every frame
void AWorkerUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch(WorkPlace)
	{
		case EWorkPlace::STRUCTURE:
			//Let the structure control the player
			break;
		case EWorkPlace::GATHERING:
			//Automate gathering
			break;
		case EWorkPlace::NONE:
			//DO nothing
			break;
		
	}
	
	if (bIsMoving) {
		AIController->MoveToLocation(Destination, 10.f);
	}
}

// Called to bind functionality to input
void AWorkerUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWorkerUnit::GetStructure(AStructure* unit)
{
	if(MyStructure && MyStructure != unit)
	{
		RemoveStructure();
	}
	MyStructure = unit;
	WorkPlace = EWorkPlace::STRUCTURE;
}

void AWorkerUnit::RemoveStructure()
{
	if(MyStructure)
	{
		MyStructure->RemoveWorker(this);
	}
	MyStructure = nullptr;
}

void AWorkerUnit::GetDestination(FVector newDestination)
{
	bIsMoving = true;
	Destination = newDestination;

}

void AWorkerUnit::StopMoving()
{
	bIsMoving = false;
}

void AWorkerUnit::ChangeMesh()
{
	//Start array at correct depening on what gender
	int mesh;
	if (bIsMale) {
		mesh = 0;
	}
	else {
		mesh = 3;
	}

	//Find what stage they are in
	mesh += Stage;
	WorkerMesh->SetStaticMesh(BodyMeshes[mesh]);
}

float AWorkerUnit::DistanceFromDestination()
{
	return FVector::Distance(GetActorLocation(), Destination);
}

bool AWorkerUnit::InRange(float min)
{
	float distance = FVector::Distance(GetActorLocation(), Destination);
	if(distance <= min)
	{
		return true;
	}else
	{
		return false;
	}
}

void AWorkerUnit::GetGatherableUnit(AGatherableUnit* unit)
{
	WorkPlace = EWorkPlace::GATHERING;
	if(MyStructure)
	{
		MyStructure->RemoveWorker(this);
	}
}