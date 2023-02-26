// Fill out your copyright notice in the Description page of Project Settings.


#include "GatherableHandler.h"
#include "GatherableUnit.h"
#include "WorkerUnit.h"
#include "BuildingManager.h"
#include "Structure.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AGatherableHandler::AGatherableHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGatherableHandler::BeginPlay()
{
	Super::BeginPlay();
	//Get refrences to all gatherable untis
	GetGatherableUnits();
}

// Called every frame
void AGatherableHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGatherableHandler::GetGatherableUnits()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGatherableUnit::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		GatherableUnits.Add(Cast<AGatherableUnit>(FoundActors[i]));
	}
}

AGatherableUnit* AGatherableHandler::GetGatherable(AWorkerUnit* w, ResourceType t)
{
	//Get typoe
	TArray<AGatherableUnit*> CorrectTypes;
	for (int i = 0; i < GatherableUnits.Num(); i++)
	{
		if(GatherableUnits[i]->MyType == t)
		{
			CorrectTypes.Add(GatherableUnits[i]);
		}
	}

	if (CorrectTypes.Num() < 1)
	{
		return nullptr;
	}
	
	AGatherableUnit* closest = CorrectTypes[0];
	for (int i = 1; i < CorrectTypes.Num(); i++)
	{
		if(FVector::Distance(CorrectTypes[i]->GetActorLocation(), w->GetActorLocation()) < FVector::Distance(closest->GetActorLocation(), w->GetActorLocation()))
		{
			closest = CorrectTypes[i];
		}
	}

	return closest;
}
TArray<AGatherableUnit*> AGatherableHandler::AllGatherableUnitsOfType(ResourceType t)
{
	TArray<AGatherableUnit*> units;
	for (int i = 0; i < GatherableUnits.Num(); i++)
	{
		if (GatherableUnits[i]->MyType == t)
		{
			units.Add(GatherableUnits[i]);
		}
	}
	return units;
}
AStructure* AGatherableHandler::GetClosestStructure(AWorkerUnit*, ResourceType)
{
	return nullptr;
}