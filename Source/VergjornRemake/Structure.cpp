// Fill out your copyright notice in the Description page of Project Settings.


#include "Structure.h"
#include "Worker.h"
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
		for (int i = 0; i < workers.Num(); i++)
		{
			workers[i]->MyState = AWorker::Working;
		}
	}
}

bool AStructure::IAmWorking() {
	if (workers.Num() > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool AStructure::GetWorkers(TArray<AWorker*> units) {
	for (int i = 0; i < units.Num(); i++)
	{
		if (!workers.Contains(units[i])) {
			workers.Add(units[i]);
			UE_LOG(LogTemp, Log, TEXT("Got a worker"));
			//Say to worker you are taken
		}
	}
	return true;
}

void AStructure::RemoveWorkers(TArray<AWorker*> units) {
	for (int i = 0; i < units.Num(); i++)
	{
		if (workers.Contains(units[i])) {
			workers.RemoveSingle(units[i]);
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