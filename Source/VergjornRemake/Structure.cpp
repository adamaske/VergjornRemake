// Fill out your copyright notice in the Description page of Project Settings.


#include "Structure.h"
#include "WorkerUnit.h"
// Sets default values
AStructure::AStructure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AStructure::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IAmWorking()) {
		//Do working stuff
		for (int i = 0; i < WorkerUnits.Num(); i++)
		{
			WorkerUnits[i]->MyState = AWorkerUnit::Working;
		}
	}
}

bool AStructure::IAmWorking() {
	if (WorkerUnits.Num() > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool AStructure::GetWorkerUnits(TArray<AWorkerUnit*> units) {
	for (int i = 0; i < units.Num(); i++)
	{
		if (!WorkerUnits.Contains(units[i])) {
			WorkerUnits.Add(units[i]);
			UE_LOG(LogTemp, Log, TEXT("Got a WorkerUnit"));
			//Say to WorkerUnit you are taken
		}
	}
	return true;
}

void AStructure::RemoveWorkerUnits(TArray<AWorkerUnit*> units) {
	for (int i = 0; i < units.Num(); i++)
	{
		if (WorkerUnits.Contains(units[i])) {
			WorkerUnits.RemoveSingle(units[i]);
		}
	}
}

void AStructure::TakeDamage(float damage)
{
	CurrentHealth -= damage;
	if (CurrentHealth <= 0) {
		bIsDead = true;
	}
}