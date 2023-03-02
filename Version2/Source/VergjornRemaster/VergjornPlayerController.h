// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "VergjornPlayerController.generated.h"


/** Forward declaration to improve compiling times */
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class VERGJORNREMASTER_API AVergjornPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AVergjornPlayerController();


	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	//Quick saving and loading
	void QuickSave();
	void QuickLoad();
};
