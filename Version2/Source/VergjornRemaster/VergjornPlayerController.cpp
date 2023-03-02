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

	InputComponent->BindAction("QuickSave", IE_Pressed, this, &AVergjornPlayerController::QuickSave);

	InputComponent->BindAction("QuickLoad", IE_Pressed, this, &AVergjornPlayerController::QuickLoad);

}

void AVergjornPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AVergjornPlayerController::QuickSave()
{
	auto vergjorn = Cast<UVergjornGameInstance>(GetGameInstance());
	if (!vergjorn) {
		return;
	}

	//When quick saving we want to save the active map

	vergjorn->Save();
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("PC Quick Saved!")));
}

void AVergjornPlayerController::QuickLoad()
{
	auto vergjorn = Cast<UVergjornGameInstance>(GetGameInstance());
	if (!vergjorn) {
		return;
	}
	vergjorn->LoadMostRecentSaveGame();
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("PC Quick Loaded!")));
}
