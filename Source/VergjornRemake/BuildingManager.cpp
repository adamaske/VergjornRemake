// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingManager.h"
#include "Structure.h"
#include "PlayerUnit.h"
// Sets default values
ABuildingManager::ABuildingManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuildingManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ABuildingManager::BuyStructure(AStructure* unit)
{
	for (int i = 0; i < unit->Costs.Num(); i++)
	{
		if (unit->Costs[i].Type.myType == ResourceType::METAL) {

		}
	}

	return false;
}

void ABuildingManager::GetPlayer(APlayerUnit* unit)
{
	MyPlayer = unit;
}

