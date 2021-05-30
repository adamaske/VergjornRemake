// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BuildingManager.generated.h"

struct FResource;
USTRUCT(BlueprintType)
struct FStructureInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stuff)
	UTexture2D* Image;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stuff)
		FString Name;
};
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
	
	

	//PLayer refrence
	void GetPlayer(class APlayerUnit*);
	APlayerController* PC;
	UPROPERTY(EditAnywhere)
	APlayerUnit* MyPlayer;
	
	//Buiying
	UFUNCTION(BlueprintCallable)
	bool BuyStructure(class AStructure* structure);
	void DoPurchase(class AStructure*);

	//Building
	AStructure* CurrentStructure;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building")
	bool bIsBuilding;
	UFUNCTION(BlueprintCallable)
	void Build();

	//Placing Visual and Register Placement
	UFUNCTION(BlueprintCallable)
	void Place();
	UFUNCTION(BlueprintCallable)
	void StopPlacing();
	//Visual
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* VisualMeshComponent;
	
	//Buildings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
		TArray<TSubclassOf<AStructure>> Buildings;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int BuildingsCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Building")
		TArray<const AStructure*> BPStructures;
};


