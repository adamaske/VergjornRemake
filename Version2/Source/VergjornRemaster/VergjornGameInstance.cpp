// Fill out your copyright notice in the Description page of Project Settings.


#include "VergjornGameInstance.h"

#include "VergjornSaveAndLoad.h"
#include "VergjornSaveGame.h"
#include "SaveGameLoaderActor.h"
#include "SaveGameCreatorActor.h"
#include "Kismet/GameplayStatics.h"
#include <string>

//void UVergjornGameInstance::Init()
//{
//	VergjornStarted();
//}
static VergjornConfig sConfig;
void UVergjornGameInstance::VergjornStarted()
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("Vergjorn Started")));

	//Here we may want to load a config file ? 
	//Config file time
	auto config = VergjornSaveAndLoad::LoadVergjornConfigFile("VergjornConfig");
	if (config.bEmptyConfig) {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Did not find Config file")));

		//Then we create one
		VergjornConfig con;
		con.bEmptyConfig = false;
		con.mMostRecentSaveGame = 0;
		VergjornSaveAndLoad::CreateVergjornConfigFile("VergjornConfig", con);
		config = VergjornSaveAndLoad::LoadVergjornConfigFile("VergjornConfig");
	}
	sConfig = config;

	ApplyConfig();

	//
	auto vergjornSaves = LoadVergjornSaves();
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("Amount of Vergjorn Saves Loaded : %f"), vergjornSaves.size()));

}

std::vector<UVergjornSaveGame*> UVergjornGameInstance::LoadVergjornSaves()
{
	std::vector<UVergjornSaveGame*> saves;
	int checkRange = 100;
	for (int i = 0; i < checkRange; i++) {
		auto save = VergjornSaveAndLoad::Load("VergjornSave_" + FString::FromInt(i), 0);
		if (save) {
			saves.push_back(save);
			mVergjornSaves[i] = save;
		}
	}
	//Stored 
	
	return saves;
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
	UVergjornSaveGame* save = Cast<UVergjornSaveGame>(UGameplayStatics::CreateSaveGameObject(UVergjornSaveGame::StaticClass()));
	if (!save) {
		//
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Save Game could not be created!")));
		return;
	}
	if (!mCreatorActor) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("No Creator Actor!")));
		return;
	}
	mCreatorActor->GetVergjornSaveGameFromWorld(save);


	SaveGameIncremented(save, TCHAR_TO_UTF8(std::string("VergjornSave_" + std::to_string(0)).c_str()), 0);
}

void UVergjornGameInstance::LoadMostRecentSaveGame()
{
	bool loaded = mLoaderActor->LoadSave(VergjornSaveAndLoad::Load(TCHAR_TO_UTF8(std::string("VergjornSave_" + std::to_string(mNewestSaveIndex)).c_str()), 0));
	if (!loaded) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("GI Load not completed!")));
	}
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("GI Load Completed!")));

}

void UVergjornGameInstance::ApplyConfig()
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("Applying Config File")));

	mLastActiveScene = sConfig.mMostRecentSaveGame;
}

