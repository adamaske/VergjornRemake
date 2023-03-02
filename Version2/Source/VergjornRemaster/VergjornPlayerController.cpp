// Fill out your copyright notice in the Description page of Project Settings.


#include "VergjornPlayerController.h"
#include "VergjornGameInstance.h"
#include "VergjornSaveAndLoad.h"
#include "VergjornSaveGame.h"
AVergjornPlayerController::AVergjornPlayerController()
{
		bShowMouseCursor = true;
}

void AVergjornPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("Save", IE_Pressed, this, &AVergjornPlayerController::Save);

	InputComponent->BindAction("Load", IE_Pressed, this, &AVergjornPlayerController::Load);

}

void AVergjornPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AVergjornPlayerController::Save()
{
	auto vergjorn = Cast<UVergjornGameInstance>(GetGameInstance());
	if (vergjorn) {

	}
}

void AVergjornPlayerController::Load()
{

}
