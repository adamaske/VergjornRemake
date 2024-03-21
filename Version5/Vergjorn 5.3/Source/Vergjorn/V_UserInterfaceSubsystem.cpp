// Fill out your copyright notice in the Description page of Project Settings.


#include "V_UserInterfaceSubsystem.h"
#include "VergjornSubsystem.h"

void UV_UserInterfaceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("UI_SS: DECAPERATED Subsystem")));

	//Depedent on other subsystems to be already launched
	Collection.InitializeDependency<UVergjornSubsystem>();
}

void UV_UserInterfaceSubsystem::Deinitialize()
{
}
