// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingManager.h"

#include <string>

#include "Structure.h"
#include "PlayerUnit.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ABuildingManager::ABuildingManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VisualMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	//VisualMeshComponent->SetCollisionEnabled(ECollisionShapeType::None);
}

// Called when the game starts or when spawned
void ABuildingManager::BeginPlay()
{
	Super::BeginPlay();
	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	//Create arrays of Structure* for the blueprint to be able to get its variables
	BuildingsCount = Buildings.Num();
	BPStructures.SetNum(Buildings.Num());
	for (int i = 0; i < Buildings.Num(); i++)
	{
		//New object to create a AStructure in memory
		AStructure* a = NewObject<AStructure>();
		//DefultObject uses the default blueprint values of the TSubclassOf<AStructure> inBuildings
		a = Buildings[i].GetDefaultObject();
		BPStructures[i] = a;
	}
}

// Called every frame
void ABuildingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsBuilding)
	{
		//Stop building players blueprint
		Place();
	}
}

bool ABuildingManager::BuyStructure(AStructure* unit)
{
	float checks = 0;
	bool cantAfford = false;
	//Check every cost that the structure has
	for (int i = 0; i < unit->Costs.Num(); i++)
	{
		
		//Check all the players resource amounts
		for (auto j = 0; j < MyPlayer->ResourceAmounts.Num(); j++)
		{
			//Find the correct type
			if(MyPlayer->ResourceAmounts[j].myType == unit->Costs[i].Type)
			{
				checks++;
				//checks++;
				//Find if the player has enough of it
				if(MyPlayer->ResourceAmounts[j].Amount < unit->Costs[i].Amount)
				{
					cantAfford = true;
					UE_LOG(LogTemp, Log, TEXT("What failed: %s"), MyPlayer->ResourceAmounts[j].myType);
				}
			}
		}
	}
	//Didnt check any resources or the player had none equal to the costs
	if(checks == 0 || cantAfford)
	{
		UE_LOG(LogTemp, Log, TEXT("Didnt check anything"));
		return false;
	}
	
	//Return true if came trough all of 
	DoPurchase(unit);
	return true;
}

void ABuildingManager::DoPurchase(AStructure* unit)
{
	bIsBuilding = true;
	CurrentStructure = unit;
	VisualMeshComponent->SetStaticMesh(CurrentStructure->MainMesh->GetStaticMesh());
	VisualMeshComponent->SetVisibility(true);

	//Check every cost that the structure has
	for (int i = 0; i < unit->Costs.Num(); i++)
	{
		//Check all the players resource amounts
		for (int j = 0; j < MyPlayer->ResourceAmounts.Num(); j++)
		{
			//Find the correct type
			if (MyPlayer->ResourceAmounts[j].myType == unit->Costs[i].Type)
			{
				//Find if the player has enough of it
				if (MyPlayer->ResourceAmounts[j].Amount >= unit->Costs[i].Amount)
				{
					//ALLOW TO BUY

					//Take away resource amount
					MyPlayer->ResourceAmounts[j].Amount -= unit->Costs[i].Amount;
				}
			}
		}
	}
}

void ABuildingManager::Place()
{
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if(!HitResult.GetActor())
	{
		//UE_LOG(LogTemp, Log, TEXT("No actor under HItResult from building"));
		return;
	}
	
	VisualMeshComponent->SetWorldLocation(HitResult.Location);
	
	//VisualMeshComponent->SetWorldRotation(hit.Normal);
}
void ABuildingManager::StopPlacing()
{
	VisualMeshComponent->SetVisibility(false);
	bIsBuilding = false;
}

void ABuildingManager::Build()
{
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	FVector Location = HitResult.Location;

	//NEEDS BETTER METHOD OF SPAWNING IT THAN COMPARING NAME, THIS SHOULD NEVER FAIL BUT SHOULD BE REPLCAED
	for (int i = 0; i < Buildings.Num(); i++)
	{
		if(Buildings[i].GetDefaultObject()->StructureName == CurrentStructure->StructureName)
		{
			AStructure* newStructure = GetWorld()->SpawnActor<AStructure>(Buildings[i], Location, FRotator(0, 0, 0));
		}
	}
}
void ABuildingManager::GetPlayer(APlayerUnit* a)
{
	//Sets player
	MyPlayer = a;
}

