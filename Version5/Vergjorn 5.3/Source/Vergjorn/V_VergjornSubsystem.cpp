// Fill out your copyright notice in the Description page of Project Settings.


#include "V_VergjornSubsystem.h"


void UV_VergjornSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("V_SS: Vergjorn Subsystem")));

	//Depedent on other subsystems to be already launched
	//Collection.InitializeDependency<AnotherSubSystem>()
}

void UV_VergjornSubsystem::Deinitialize()
{
}

void UV_VergjornSubsystem::Test()
{

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("V_SS: Test")));
}
