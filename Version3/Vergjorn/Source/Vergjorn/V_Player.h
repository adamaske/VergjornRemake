// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "V_Player.generated.h"

class UCameraComponent;
class UFloatingPawnMovement;
UCLASS()
class VERGJORN_API AV_Player : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AV_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* mCamera;


#pragma region Normal Movement and Rotation
private:
	
	UFloatingPawnMovement* mFloatingPawnMovement;
	//FPS MOVEment
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float mMoveSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float mTurnRate = 45.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float mLookUpRate = 45.f;

	void MoveForward(float value);
	void MoveRight(float value);

	void Turn(float value);
	void LookUp(float value);
#pragma endregion
};
