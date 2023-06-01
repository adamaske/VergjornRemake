// Fill out your copyright notice in the Description page of Project Settings.


#include "V_PlayerController.h"

void AV_PlayerController::BeginPlay() {

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
}

void AV_PlayerController::RightClick_Pressed()
{
}

void AV_PlayerController::RightClick_Released()
{
}
void AV_PlayerController::LeftClick_Pressed()
{
}

void AV_PlayerController::LeftClick_Released()
{
}
