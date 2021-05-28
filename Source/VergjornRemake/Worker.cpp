// Fill out your copyright notice in the Description page of Project Settings.


#include "Worker.h"
#include "Structure.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
AWorker::AWorker()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	WorkerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMainMesh"));
	WorkerMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWorker::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
}

// Called every frame
void AWorker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	 
	if (bIsMoving) {
		AIController->MoveToLocation(Destination, 10.f, true, true);
	}
}

// Called to bind functionality to input
void AWorker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWorker::GetStructure(AStructure* unit)
{
	MyStructure = unit;
}

void AWorker::RemoveStructure()
{
	MyStructure = nullptr;
}

void AWorker::GetDestination(FVector newDestination)
{
	bIsMoving = true;
	Destination = newDestination;
	UE_LOG(LogTemp, Log, TEXT("I GOT DESTINATION"));
}

void AWorker::StopMoving()
{
	bIsMoving = false;
}
