// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerUnit.h"
#include "GatherableHandler.generated.h"

UCLASS()
class VERGJORNREMAKE_API AGatherableHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGatherableHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<class AGatherableUnit*> GatherableUnits;
	
	void GetGatherableUnits();

	AGatherableUnit* GetGatherable(class AWorkerUnit*, ResourceType);
	class AStructure* GetClosestStructure(AWorkerUnit*, ResourceType);
	class ABuildingManager* BuildingManager;

	TArray<class AGatherableUnit*> AllGatherableUnitsOfType(ResourceType);
};
