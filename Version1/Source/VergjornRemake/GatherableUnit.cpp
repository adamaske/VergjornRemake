// Fill out your copyright notice in the Description page of Project Settings.


#include "GatherableUnit.h"
#include "PlayerUnit.h"
#include "WorkerUnit.h"
// Sets default values
AGatherableUnit::AGatherableUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGatherableUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGatherableUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
bool AGatherableUnit::GetWorker(AWorkerUnit* unit)
{
	if(!MyWorker)
	{
		MyWorker = unit;
		MyWorker->MyGatherableUnit = this;
		return true;
	}else
	{
		return false;
	}
}
void AGatherableUnit::RemoveWorker(AWorkerUnit* unit)
{
	if(MyWorker == unit)
	{
		MyWorker->MyGatherableUnit = nullptr;
		MyWorker = nullptr;
	}
}
bool AGatherableUnit::HasWorker()
{
	if(MyWorker)
	{
		return true;
	}else
	{
		return false;
	}
}