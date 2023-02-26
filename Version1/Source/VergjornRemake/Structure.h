// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerUnit.h"
#include "Structure.generated.h"

class APlayerUnit;
USTRUCT(BlueprintType)
struct FConstructionCost
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = Stuff)
		ResourceType Type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stuff)
		float Amount{0};
};
enum ResrouceType;
UCLASS()
class VERGJORNREMAKE_API AStructure : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStructure();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MainMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		FString StructureName{ "Structure" };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		FString StructureDescription{ "Description" };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		UTexture2D* StructureImage;
	//Health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	float CurrentHealth{ 100.f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	float MaxHealth{ 100.f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	bool bIsDead{ 0 };

	//void TakeDamage(float);

	//Getting and remvoing WorkerUnits
	bool GetWorkerUnits(TArray<class AWorkerUnit*>);
	bool GetWorkerUnits(AWorkerUnit*);
	void RemoveWorkerUnits(TArray<class AWorkerUnit*>);
	void RemoveWorker(AWorkerUnit*);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		TArray<class AWorkerUnit*> WorkerUnits;
	bool IAmWorking();
	
	//Player refrence
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	class APlayerUnit* MyPlayer;
	void GetPlayer(APlayerUnit*);
	
	//Resources
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	float ResourceAmountToGive{ 10 };
	void MakeResource();
	//Building
	class ABuildingManager* BuildingManager;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		TArray<FConstructionCost> Rewards;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		TArray<FConstructionCost> Costs;

	//Custom features
	virtual void SelectedByPlayer();
	virtual void DeselectedByPlayer();
	//Gathering
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class AGatherableHandler* GatherableHandler;
	TArray<class AGatherableUnit*> GatherableUnits;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		ResourceType GatheringType = ResourceType::GOLD;
	
};
