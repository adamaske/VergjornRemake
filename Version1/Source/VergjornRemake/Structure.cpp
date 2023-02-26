// Fill out your copyright notice in the Description page of Project Settings.


#include "Structure.h"
#include "WorkerUnit.h"
#include "PlayerUnit.h"
#include "Kismet/GameplayStatics.h"
#include "GatherableHandler.h"
#include "GatherableUnit.h"
#include "BuildingManager.h"
// Sets default values
AStructure::AStructure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AStructure::BeginPlay()
{
	Super::BeginPlay();
	//MyPlayer = Cast<APlayerUnit>(UGameplayStatics::GetPlayerController(this, 0));
	//Find refrences
	if(!MyPlayer)
	{
		MyPlayer = Cast<APlayerUnit>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
	if (!GatherableHandler)
	{
		GatherableHandler = Cast<AGatherableHandler>(UGameplayStatics::GetActorOfClass(GetWorld(), AGatherableHandler::StaticClass()));
		GatherableUnits = GatherableHandler->AllGatherableUnitsOfType(GatheringType);
	}
	if(!BuildingManager)
	{
		BuildingManager = Cast<ABuildingManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildingManager::StaticClass()));
	}
}

// Called every frame
void AStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AStructure::IAmWorking() {
	if (WorkerUnits.Num() > 0) {
		return true;
	}
	
	return false;

}

void AStructure::GetPlayer(APlayerUnit* unit)
{
	MyPlayer = unit;
}

void AStructure::MakeResource()
{
	if(MyPlayer)
	{
		for (int i = 0; i < Rewards.Num(); i++)
		{
			MyPlayer->GetResources(Rewards[i].Type, Rewards[i].Amount);
		}
	}else
	{
		UE_LOG(LogTemp, Log, TEXT("No Player"));
	}
}

bool AStructure::GetWorkerUnits(AWorkerUnit* unit)
{
	if(!WorkerUnits.Contains(unit))
	{
		WorkerUnits.Add(unit);
		unit->GetStructure(this);
		UE_LOG(LogTemp, Log, TEXT("Got a WorkerUnit"));
		return true;
	}
	return false;
}

bool AStructure::GetWorkerUnits(TArray<AWorkerUnit*> units) {
	for (int i = 0; i < units.Num(); i++)
	{
		if (!WorkerUnits.Contains(units[i])) {
			WorkerUnits.Add(units[i]);
			units[i]->GetStructure(this);
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

void AStructure::RemoveWorker(AWorkerUnit* unit)
{
	if(WorkerUnits.Contains(unit))
	{
		WorkerUnits.RemoveSingle(unit);
		UE_LOG(LogTemp, Log, TEXT("Removed worker"));
	}
}

void AStructure::SelectedByPlayer()
{
	//Custom ui or stuff
}

void AStructure::DeselectedByPlayer()
{
	//Disabel custom stuff
}