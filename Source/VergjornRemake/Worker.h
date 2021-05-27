// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Worker.generated.h"

UCLASS()
class VERGJORNREMAKE_API AWorker : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWorker();
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

	FVector Destination;
	
	enum WorkerState{Moving, Idle, Working};
	WorkerState MyState;
};
