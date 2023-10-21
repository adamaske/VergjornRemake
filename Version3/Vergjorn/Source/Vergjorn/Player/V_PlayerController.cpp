// Fill out your copyright notice in the Description page of Project Settings.


#include "V_PlayerController.h"
#include "V_Player.h"

#include "../V_HUD.h"
#include "../EntitySelector.h"

void AV_PlayerController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	mPlayer = Cast<AV_Player>(pawn);
	if (!mPlayer) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("PC : Found no Player")));
	}
}

void AV_PlayerController::BeginPlay() {

	//Set mouse
	SetShowMouseCursor(true);
	//Create HUD
	
	//Add HUD to viewport
	mHUD = Cast<AV_HUD>(GetHUD());
	if (!mHUD) {
		//Somethign wrong
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("PC : Found no HUD")));
	}

	//Spawns entity selector
	//TODO Change this from actor to something else, this doesnt need to live in the scene I think

	mEntitySelector = Cast<AEntitySelector>(GetWorld()->SpawnActor(mEntitySelectorBP ? mEntitySelectorBP : AEntitySelector::StaticClass()));

}
void AV_PlayerController::Tick(float DeltaTime) {


}

void AV_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("RightClick", IE_Pressed, this, &AV_PlayerController:: RightClick_Pressed );
	InputComponent->BindAction("RightClick", IE_Released, this, &AV_PlayerController::RightClick_Released);


	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AV_PlayerController:: LeftClick_Pressed );
	InputComponent->BindAction("LeftClick", IE_Released, this, &AV_PlayerController::LeftClick_Released);

	InputComponent->BindAxis("AddativeSelection", this, &AV_PlayerController::SetAddativeSelectionType);
}

void AV_PlayerController::RightClick_Pressed()
{
	if (mPlayer) {
		mPlayer->EnableRotation();
	}
}

void AV_PlayerController::RightClick_Released()
{
	if (mPlayer) {
		mPlayer->DisableRotation();
	}
}
void AV_PlayerController::LeftClick_Pressed()
{
	//Select
	if (mEntitySelector) {
		mEntitySelector->StartSelection();
	}
	if (mHUD) {
		mHUD->MarqueeStart();
	}
}


void AV_PlayerController::LeftClick_Released()
{
	//Stop Select
	if (mEntitySelector) {
		mEntitySelector->StopSelection();
	}
	
	if (mHUD) {
		mHUD->MarqueeEnded();
	}
}
void AV_PlayerController::SetAddativeSelectionType(float value){

	if (mEntitySelector) {
		mEntitySelector->SetAddative(value);
	}
}