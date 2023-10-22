// Fill out your copyright notice in the Description page of Project Settings.


#include "Vergjorn_Instance.h"

UVergjorn_Instance::UVergjorn_Instance()
{
}

void UVergjorn_Instance::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("GI Init")));
	
	//Create Vergjorn
	//mVergjorn = NewObject<UV_Vergjorn>();

	//Vergjorn
	//mVergjorn->Init();

}

void UVergjorn_Instance::Shutdown()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("GI Shutdown")));


	//mVergjorn->Shutdown();


}

void UVergjorn_Instance::RegisterGameMode(AVergjornGameModeBase* gm)
{

}
