// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerUnit.generated.h"

UCLASS()
class VERGJORNREMAKE_API APlayerUnit : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerUnit();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
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
	FVector StartSelectionLocation;
	FVector EndSelectionLocation;
	bool bIsSelecting{ 0 };
	void Interact();
	//Workers
	TArray<class AWorker*> WorkersSelected;


};
