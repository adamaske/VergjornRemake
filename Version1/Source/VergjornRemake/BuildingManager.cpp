// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingManager.h"
#include "CapacityComponent.h"
#include "Structure.h"
#include "PlayerUnit.h"
#include "WorkerUnit.h"
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
	
	CreateRefrences();
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

void ABuildingManager::CreateRefrences()
{
	//Create arrays of Structure* for the blueprint to be able to get its variables
	BuildingsCount = Buildings.Num();
	BPStructures.SetNum(Buildings.Num());

	//Fills array of memory-only Structures which then are refrenced with correct information
	for (int i = 0; i < Buildings.Num(); i++)
	{
		//New object to create a AStructure in memory
		AStructure* a = NewObject<AStructure>(Buildings[i]);
		//DefultObject uses the default blueprint values of the TSubclassOf<AStructure> inBuildings
		a = Buildings[i].GetDefaultObject();
		BPStructures[i] = a;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStructure::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		Structures.Add(Cast<AStructure>(FoundActors[i]));
	}
	ReloadCapacity();
}

bool ABuildingManager::BuyStructure(AStructure* unit)
{
	if(bIsBuilding)
	{
		StopPlacing();
	}
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
				//Find if the player has enough of it
				if(MyPlayer->ResourceAmounts[j].Amount < unit->Costs[i].Amount)
				{
					cantAfford = true;
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
			Structures.Add(newStructure);
			newStructure->MyPlayer = MyPlayer;
		}
	}
	ReloadCapacity();
}

void ABuildingManager::GetPlayer(APlayerUnit* a)
{
	//Sets player
	MyPlayer = a;
}

void ABuildingManager::ReloadCapacity()
{
	//Reset Capacities
	CapacityComponents.Empty();
	for (int i = 0; i < Structures.Num(); i++)
	{
		UCapacityComponent* component;
		component = Cast<UCapacityComponent>(Structures[i]->GetComponentByClass(UCapacityComponent::StaticClass()));
		if(component)
		{
			CapacityComponents.Add(component);
		}
	}
	//Clearing all the capacityies
	for (int j = 0; j < MyPlayer->ResourceAmounts.Num(); j++)
	{
		MyPlayer->ResourceAmounts[j].ExtraCapacity = 0;
	}
	
	for (int i = 0; i < CapacityComponents.Num(); i++)
	{
		for(int k = 0; k < CapacityComponents[i]->MyAmounts.Num(); k++)
		{
			for (int j = 0; j < MyPlayer->ResourceAmounts.Num(); j++)
			{
				if(CapacityComponents[i]->MyAmounts[k].myType == MyPlayer->ResourceAmounts[j].myType)
				{
					MyPlayer->ResourceAmounts[j].ExtraCapacity += CapacityComponents[i]->MyAmounts[k].Amount;
				}
			}
		}
	}

}

UCapacityComponent* ABuildingManager::GetClosestCapacity(ResourceType t , AWorkerUnit* w)
{
	TArray<UCapacityComponent*> CorrectTypes;
	for (int i = 0; i < CapacityComponents.Num(); i++)
	{
		for (int k = 0; k < CapacityComponents[i]->MyAmounts.Num(); k++)
		{
			if(CapacityComponents[i]->MyAmounts[k].myType == t)
			{
				CorrectTypes.Add(CapacityComponents[i]);
			}
		}
	}
	if(CorrectTypes.Num() < 1)
	{
		return nullptr;
	}
	//Find closest to player'
	UCapacityComponent* closest = CorrectTypes[0];
	float Distance = FVector::Distance(closest->GetOwner()->GetActorLocation(), w->GetActorLocation());
	for (int i = 1; i < CorrectTypes.Num(); i++)
	{
		float newDistance = FVector::Distance(CorrectTypes[i]->GetOwner()->GetActorLocation(), w->GetActorLocation());
		if(newDistance < Distance )
		{
			Distance = newDistance;
			closest = CorrectTypes[i];
		}
	}

	return closest;
}