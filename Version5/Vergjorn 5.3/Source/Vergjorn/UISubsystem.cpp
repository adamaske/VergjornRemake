// Fill out your copyright notice in the Description page of Project Settings.


#include "UISubsystem.h"
#include "VergjornSubsystem.h"
void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("UI_SS: UI Subsystem")));

	//Depedent on other subsystems to be already launched
	Collection.InitializeDependency<UVergjornSubsystem>();
}

void UUISubsystem::Deinitialize()
{

}
