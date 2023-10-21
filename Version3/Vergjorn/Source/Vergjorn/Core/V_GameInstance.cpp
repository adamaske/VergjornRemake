// Fill out your copyright notice in the Description page of Project Settings.


#include "V_GameInstance.h"
#include "Vergjorn.h"
#include "V_Vergjorn.h"
UV_GameInstance::UV_GameInstance()
{
	
};

void UV_GameInstance::Init()
{
	GEngine->AddOnScreenDebugMessage(GI_INIT, 5, FColor::Green, FString::Printf(TEXT("GI Init")));
	//Create Vergjorn
	mVergjorn = NewObject<UV_Vergjorn>();

	//Vergjorn
	mVergjorn->Init();
	
}

void UV_GameInstance::Shutdown()
{
	GEngine->AddOnScreenDebugMessage(GI_SHUTDOWN, 5, FColor::Green, FString::Printf(TEXT("GI Shutdown")));

	mVergjorn->Shutdown();
	
}

UV_Vergjorn* UV_GameInstance::GetVergjorn()
{
	return mVergjorn.Get();
}