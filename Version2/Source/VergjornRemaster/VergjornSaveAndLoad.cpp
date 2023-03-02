// Fill out your copyright notice in the Description page of Project Settings.


#include "VergjornSaveAndLoad.h"
#include "JsonObjectConverter.h"
#include "VergjornSaveGame.h"
#include <string>
#include <fstream>
#include "Kismet/GameplayStatics.h"
#include <Serialization/JsonSerializer.h>
static std::string VergjornSavePrefix = "VergjornSave";

VergjornSaveAndLoad::VergjornSaveAndLoad()
{
}

VergjornSaveAndLoad::~VergjornSaveAndLoad()
{
}

void VergjornSaveAndLoad::Save(UVergjornSaveGame* g, FString name, int index)
{

	//Saving
	auto save = Cast<UVergjornSaveGame>(UGameplayStatics::CreateSaveGameObject(UVergjornSaveGame::StaticClass()));
	if (!save) {
		//Did not manage to create a vergjorn save game
	}
	//FAsyncSaveGameToSlotDelegate savedDelegate;
	//savedDelegate.BindRaw(this, &VergjornSaveAndLoad::CompletedSave);

	save->MapName = "Volugrimar";

	UGameplayStatics::SaveGameToSlot(save, name, index);

}

void VergjornSaveAndLoad::CompletedSave(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
}

UVergjornSaveGame* VergjornSaveAndLoad::Load(FString name, int index)
{
	GEngine->AddOnScreenDebugMessage(3, 10, FColor::Cyan, FString::Printf(TEXT("Load slot")));

	auto save = Cast<UVergjornSaveGame>(UGameplayStatics::LoadGameFromSlot(name, index));
	if (save) {

		GEngine->AddOnScreenDebugMessage(3, 10, FColor::Green, FString::Printf(TEXT("Loaded slot")));
		return save;
	}

	GEngine->AddOnScreenDebugMessage(3, 10, FColor::Red, FString::Printf(TEXT("Didnt find slot")));
	return nullptr;
}

std::string VergjornSaveAndLoad::GetSaveFileName(int index)
{
	std::string base = VergjornSavePrefix;
	base += "_";
	base += "0";
	base += ".save";
	return base;
}

VergjornConfig VergjornSaveAndLoad::LoadVergjornConfigFile(std::string fileName)
{
	VergjornConfig con;
	
	std::ifstream file;

	file.open(fileName);
	if (file.is_open()) {
		std::string line;
		std::getline(file, line);

		if (line != "Vergjorn Config") {
			//Something is wwrong
			con.bEmptyConfig = true;
			return con;
		}

		std::getline(file, line);
		con.mMostRecentSaveGame = std::stoi(line);
	}

	file.close();

	return VergjornConfig();
}

void VergjornSaveAndLoad::CreateVergjornConfigFile(std::string fileName, VergjornConfig con)
{

	std::ofstream file;
	file.open(fileName);

	file << "Vergjorn Config\n";

	file << std::to_string(con.mMostRecentSaveGame);

	file.close();
}
