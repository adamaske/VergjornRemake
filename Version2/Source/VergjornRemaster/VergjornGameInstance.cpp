// Fill out your copyright notice in the Description page of Project Settings.


#include "VergjornGameInstance.h"

#include "VergjornSaveAndLoad.h"
#include "VergjornSaveGame.h"
//void UVergjornGameInstance::Init()
//{
//	VergjornStarted();
//}
void UVergjornGameInstance::VergjornStarted()
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("VergjornGameInstance : Started!")));

	//auto vergjornSaves = LoadVergjornSaves();

	//Here we may want to load a config file ? 
	
}

std::vector<std::shared_ptr<UVergjornSaveGame>> UVergjornGameInstance::LoadVergjornSaves()
{
	return std::vector<std::shared_ptr<UVergjornSaveGame>>();
}

