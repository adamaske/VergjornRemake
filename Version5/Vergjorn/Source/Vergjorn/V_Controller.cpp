// Fill out your copyright notice in the Description page of Project Settings.


#include "V_Controller.h"
#include "V_Player.h"

void AV_Controller::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	mPlayer = Cast<AV_Player>(pawn);
	if (!mPlayer) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("PC : Found no Player")));
	}
}

void AV_Controller::BeginPlay() {

	//Set mouse
	SetShowMouseCursor(true);
	//Create HUD


}
void AV_Controller::Tick(float DeltaTime) {


}

void AV_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

}
