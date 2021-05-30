// Copyright Epic Games, Inc. All Rights Reserved.


#include "VergjornRemakeGameModeBase.h"
#include "PlayerUnit.h"
#include "Structure.h"
#include "BuildingManager.h"
AVergjornRemakeGameModeBase::AVergjornRemakeGameModeBase() {
	PrimaryActorTick.bCanEverTick = true;
}

void AVergjornRemakeGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//Spawn player unit
	UWorld* World = GetWorld();
	if(!World)
	{
		return;
	}
	//Player = World->SpawnActor<APlayerUnit>(PlayerBlueprint, PlayerSpawnLocation, FRotator(0,0,0));

	//BuildingManager = World->SpawnActor<ABuildingManager>(BuildingManagerBlueprint);
	//GiveRefrences
	//Player->GetBuildingManager(BuildingManager);
	//BuildingManager->GetPlayer(Player);
	
	
	
}

void AVergjornRemakeGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
