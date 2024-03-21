// Fill out your copyright notice in the Description page of Project Settings.


#include "VergjornSubsystem.h"
#include "SaveAndLoadSubsystem.h"

void UVergjornSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("V_SS: Vergjorn Subsystem")));

	//Depedent on other subsystems to be already launched
	
}

void UVergjornSubsystem::Deinitialize()
{
}

FVergjornSaveGame UVergjornSubsystem::GetWorldAsSaveGame()
{
	FVergjornSaveGame save;

	//Get all units
	//Registry request for all units
	// 
	// 
	//
	return save;
}

void UVergjornSubsystem::SetWorldAsSaveGame(FVergjornSaveGame save)
{
	//Takes in save file and applies it to world


}
