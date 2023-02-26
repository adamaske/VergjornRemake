// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerUnit.generated.h"

UENUM(BlueprintType)
enum class ResourceType : uint8 {
	GOLD UMETA(DisplayName = "GOLD"),
	METAL = 1  UMETA(DisplayName = "METAL"),
	WOOD = 2     UMETA(DisplayName = "WOOD"),
	MYRMALM = 3		UMETA(DisplayName = "MYRMALM"),
	FOOD = 4	UMETA(DisplayName = "FOOD"),
	SHIP = 5	UMETA(DisplayName = "SHIP")
};

USTRUCT(BlueprintType)
struct FResource
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stuff)
		ResourceType myType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stuff)
		UTexture2D* ResourceImage;
};

USTRUCT(BlueprintType)
struct FResourceAmount
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stuff)
		ResourceType myType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stuff)
		float Amount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stuff)
		float BaseCapacity{ 100 };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stuff)
		float ExtraCapacity{0};
};

UCLASS()
class VERGJORNREMAKE_API APlayerUnit : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerUnit();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APlayerController* PC;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
		float CameraMovementSpeed{ 100 };
	FVector MovementVector;
	void MoveForward(float);
	void MoveRight(float);
	void MoveUp(float);

	//Selection and Interaction
	void StartSelection();
	void EndSelection();
	UFUNCTION(BlueprintImplementableEvent)
	void OnStartSelection();
	UFUNCTION(BlueprintImplementableEvent)
	void OnEndSelection();
	FVector StartSelectionLocation;
	FVector EndSelectionLocation;

	bool bIsSelecting{ 0 };
	void Interact();
	//WorkerUnits
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<class AWorkerUnit*> WorkersSelected;
	void MoveWorkerUnits(FVector);
	class AWorkerUnit* HoveredWorker;
	UPROPERTY(BlueprintReadOnly)
		bool bIsHovering;
	//General
	void CheckHover();
	FHitResult CurrentHover;
	
	//Resources
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources")
		TArray<FResourceAmount> ResourceAmounts;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources")
		TArray<FResource> Resources;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
		class ABuildingManager* BuildingManager;
	void GetBuildingManager(class ABuildingManager*);
	bool GetResources(ResourceType,float);

	//Structure
	class AStructure* SelectedStructure;
	void Select();

	//Gathering
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class AGatherableHandler* GatherableHandler;
	
};
