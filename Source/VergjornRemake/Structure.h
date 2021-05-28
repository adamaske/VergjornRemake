// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structure.generated.h"

UCLASS()
class VERGJORNREMAKE_API AStructure : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStructure();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
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

	//Health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentHealth{ 100.f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxHealth{ 100.f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bIsDead{ 0 };

	void TakeDamage(float);

	//Getting and remvoing WorkerUnits
	bool GetWorkerUnits(TArray<class AWorkerUnit*>);
	void RemoveWorkerUnits(TArray<class AWorkerUnit*>);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		TArray<class AWorkerUnit*> WorkerUnits;
	bool IAmWorking();
};
