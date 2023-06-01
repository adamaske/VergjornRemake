// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "V_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VERGJORN_API AV_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//virtual void OnPossess(APawn* pawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	//Register input 
	void RightClick_Pressed();
	void RightClick_Released();

	void LeftClick_Pressed();
	void LeftClick_Released();
};
