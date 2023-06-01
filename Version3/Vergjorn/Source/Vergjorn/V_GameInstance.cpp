// Fill out your copyright notice in the Description page of Project Settings.


#include "V_GameInstance.h"
#include "Vergjorn.h"

UV_GameInstance::UV_GameInstance()
{
	//Create Vergjorn
	mVergjorn = new Vergjorn();

};

void UV_GameInstance::Init()
{
	GEngine->AddOnScreenDebugMessage(GI_INIT, 5, FColor::Green, FString::Printf(TEXT("GI Init")));

	//What do we do in init

	//Vergjorn
	mVergjorn->Init();
	
}

void UV_GameInstance::Shutdown()
{
	GEngine->AddOnScreenDebugMessage(GI_SHUTDOWN, 5, FColor::Green, FString::Printf(TEXT("GI Shutdown")));

	mVergjorn->Shutdown();
	delete mVergjorn;
}
