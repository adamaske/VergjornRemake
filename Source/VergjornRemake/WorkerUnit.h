// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WorkerUnit.generated.h"
class AGatherableUnit;
UENUM(BlueprintType)
enum class EWorkState : uint8
{
	GATHERING,
	DELIVERING,
	BUILDING,
	IDLE
};
UENUM(BlueprintType)
enum class EWorkPlace : uint8
{
	GATHERING,
	STRUCTURE,
	NONE
};
UCLASS()
class VERGJORNREMAKE_API AWorkerUnit : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	AWorkerUnit();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* WorkerMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Structure and working
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Working")
	class AStructure* MyStructure;

	void GetStructure(class AStructure*);

	void RemoveStructure();

	//Functionanlity
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector Destination;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsMoving;
	void GetDestination(FVector);
	void StopMoving();
	class AAIController* AIController;

	//Meshes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Meshes")
	TArray<UStaticMesh*> BodyMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Meshes")
	bool bIsMale{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Meshes")
	int Stage{ 0 }; // 0 = child, 1 = villager, 2 = viking;
	void ChangeMesh();

	//General infromation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		FString WorkerName;
	
	float DistanceFromDestination();

	//Working
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		EWorkState WorkingState = EWorkState::IDLE;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		EWorkPlace WorkPlace = EWorkPlace::NONE;
	AGatherableUnit* MyGatherableUnit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	float CurrentTime{ 0 };

	bool InRange(float);

	void GetGatherableUnit(AGatherableUnit*);
};
