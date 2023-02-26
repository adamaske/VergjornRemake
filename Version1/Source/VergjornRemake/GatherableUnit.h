// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GatherableUnit.generated.h"

enum class ResourceType : uint8;

UCLASS()
class VERGJORNREMAKE_API AGatherableUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGatherableUnit();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//NEED THE STATIC GLOBAL TYPE FOR RESOURCES
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ResourceType MyType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bAllowSeveralWorkers{ 0 };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AWorkerUnit* MyWorker;
	bool GetWorker(AWorkerUnit*);
	void RemoveWorker(AWorkerUnit*);
	bool HasWorker();
};
