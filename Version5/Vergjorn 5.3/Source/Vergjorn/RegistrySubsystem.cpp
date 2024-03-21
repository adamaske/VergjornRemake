// Fill out your copyright notice in the Description page of Project Settings.


#include "RegistrySubsystem.h"
#include "VergjornSubsystem.h"

void URegistrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("R_SS: Registry Subsystem")));

	//Depedent on other subsystems to be already launched
	Collection.InitializeDependency<UVergjornSubsystem>();


}

void URegistrySubsystem::Deinitialize()
{
}


