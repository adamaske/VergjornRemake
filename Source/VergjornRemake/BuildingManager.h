// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingManager.generated.h"

UCLASS()
class VERGJORNREMAKE_API ABuildingManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	bool BuyStructure(class AStructure* structure);

	void GetPlayer(class APlayerUnit*);
	APlayerUnit* MyPlayer;

	//Buildings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
		TArray<TSubclassOf<class AStructure>> Buildings;
};
