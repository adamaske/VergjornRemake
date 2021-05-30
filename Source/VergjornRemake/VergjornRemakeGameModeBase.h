// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VergjornRemakeGameModeBase.generated.h"

/**
 * 
 */


UCLASS()
class VERGJORNREMAKE_API AVergjornRemakeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	AVergjornRemakeGameModeBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	//Player
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class APlayerUnit> PlayerBlueprint;
	class APlayerUnit* Player;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector PlayerSpawnLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class AStructure*> Structures;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class ABuildingManager> BuildingManagerBlueprint;
	ABuildingManager* BuildingManager;
};
