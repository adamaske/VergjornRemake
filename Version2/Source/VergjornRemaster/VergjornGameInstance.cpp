// Fill out your copyright notice in the Description page of Project Settings.


#include "VergjornGameInstance.h"

#include "VergjornSaveAndLoad.h"
#include "VergjornSaveGame.h"

#include <string>
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

void UVergjornGameInstance::SetActiveMapSaveGame(UVergjornSaveGame* save)
{
	std::shared_ptr<UVergjornSaveGame> _save(save);
	mActiveVergjornSaveGame = _save;
}

UVergjornSaveGame* UVergjornGameInstance::GetActiveSaveGame()
{
	return mActiveVergjornSaveGame.get();
}

void UVergjornGameInstance::SaveGameIncremented(UVergjornSaveGame* save, FString name, int userIndex)
{
	VergjornSaveAndLoad::Save(save, name, userIndex);
	mNewestSaveIndex++;
}

void UVergjornGameInstance::Save()
{
	//This function has been called so the caller havent been provided with an updated save game
	//So we gotta find the save game ourselfs
	UVergjornSaveGame* save = new UVergjornSaveGame();

	SaveGameIncremented(save, TCHAR_TO_UTF8(std::string("VergjornSave_" + std::to_string(mNewestSaveIndex)).c_str()), 0);
}

