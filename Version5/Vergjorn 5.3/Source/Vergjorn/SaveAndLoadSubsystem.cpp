// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveAndLoadSubsystem.h"
#include "VergjornSubsystem.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "JsonObjectConverter.h"
#include "HAL/FileManagerGeneric.h"
void USaveAndLoadSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("SL_SS: Save And Load Subsystem")));

}

void USaveAndLoadSubsystem::Deinitialize()
{
}

void USaveAndLoadSubsystem::Save(FVergjornSaveGame save)
{
	auto obj = SaveGameToJson(save);
	
	FString file_path = m_SaveFolderFilePath;
	file_path.Append(mSaveFile_Prefix);
	file_path.Append("-");
	file_path.Append(save.m_ClanName);
	file_path.Append("-");
	file_path.Append(save.m_DateTime.ToString());
	file_path.Append(mSaveFile_Suffix);
	
	TSharedRef<FJsonObject> json = MakeShareable<FJsonObject>(&obj);
	FString jsonString = "";
	TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&jsonString);
	//Put serialized json object into jsonString
	FJsonSerializer::Serialize(json, writer);
	
	//Then write to file
	FFileHelper::SaveStringToFile(jsonString, *file_path);
	
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Saved JSON object at : %s"), *file_path));
}

void USaveAndLoadSubsystem::Load()
{
	//What does load do?

	//Find the desired file

	//Tell the vergjorn subsystem what save file is desired

	//m_ActiveSave

}


void USaveAndLoadSubsystem::Quicksave()
{
	//Create dummy save files
	FVergjornSaveGame save = FVergjornSaveGame("Test 0", "Adam", FDateTime::Now(), 0);
	Save(save);
	
	//Save the active game file

	//VergjronSubsystem->GetSaveGame

	//Convert to json

	//Save

}

void USaveAndLoadSubsystem::Quickload()
{
}

int USaveAndLoadSubsystem::LoadAndCacheSaves()
{
	TArray<FString> fileNames;
	FFileManagerGeneric fileManager;
	fileManager.SetSandboxEnabled(true);// don't ask why, I don't know :P
	FString wildcard("*.save"); // May be "" (empty string) to search all files
	FString search_path(FPaths::Combine(m_SaveFolderFilePath, *wildcard));

	fileManager.FindFiles(fileNames, *search_path, true, false);

	//Cache json objects read from file
	TArray<FJsonObject> loadedJsonObjects; 
	for (auto name : fileNames)
	{
		FString path = m_SaveFolderFilePath;
		path.Append(name);
		FString loaded = "";
		FFileHelper::LoadFileToString(loaded, *path);//Load the file to the string

		//----------JSON DESERIALIZATION---------------
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(loaded);//reader
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);//empty object
		bool success = FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid();

		if (!success) {
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Unsuccessful Json Deserialization : %s"), *path));

			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Found  : %s"), *loaded));
			continue;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Successfull Json Deserialization : %s"), *name));

		//Add to array
		loadedJsonObjects.Add(*JsonObject);
	}
	fileNames.Empty();

	//Turn cached json objects to save games
	TArray<FVergjornSaveGame> loadedSaveGames;
	for (int i = 0; i < loadedJsonObjects.Num(); i++) {
		auto save = JsonToSaveGame(loadedJsonObjects[i]);
		loadedSaveGames.Add(save);
	}
	loadedJsonObjects.Empty();

	m_SavesArray.Empty();
	//Add to Saves
	for (auto save : loadedSaveGames) {
		m_SavesArray.Add(save);
		m_SavesMap.Add(save.m_ID, save);
	}
	int amount = loadedSaveGames.Num();
	loadedSaveGames.Empty();

	SortSaveGames(m_SavesArray);
	return amount;
}

TArray<FVergjornSaveGame> USaveAndLoadSubsystem::GetSaves()
{
	return m_SavesArray;
}

void USaveAndLoadSubsystem::CreateNewGame(FVergjornSaveGame save)
{
}


void USaveAndLoadSubsystem::SetActiveSaveGame(int ID)
{
	//NOTE : This uses the m_ID as key, this may not be appropriate in the future
	//Array version
	for (int i = 0; i < m_SavesArray.Num(); i++) {
		if (m_SavesArray[i].m_ID == ID) {
			m_ActiveSaveGame = m_SavesArray[i];
			m_ActiveSaveKey = ID;
		}

		if (i == m_SavesArray.Num() - 1) {
			//None matching found
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("SL_SS : Save Game not found in cache!")));
		}
	}

	
	//Map version
	if (auto save = m_SavesMap.Find(ID)) {
		m_ActiveSaveGame = *save;
		m_ActiveSaveKey = ID;
	}
	else {
		//None matching found
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("SL_SS : Save Game not found in cache!")));
	}


}

FJsonObject USaveAndLoadSubsystem::SaveGameToJson(FVergjornSaveGame save)
{
	FJsonObject obj;
	obj.SetStringField("Title", save.m_Title);
	obj.SetStringField("ClanName", save.m_ClanName);
	obj.SetStringField("Datetime", save.m_DateTime.ToString());
	obj.SetNumberField("ID", save.m_ID);
	return obj;
}

FVergjornSaveGame USaveAndLoadSubsystem::JsonToSaveGame(FJsonObject object)
{	
	FVergjornSaveGame game;
	game.m_Title = object.GetStringField("Title");
	game.m_ClanName = object.GetStringField("ClanName");
	bool parsed = FDateTime::Parse(object.GetStringField("Datetime"), game.m_DateTime);
	if (!parsed) {
		game.m_DateTime = FDateTime::Now();
	}
	game.m_ID = object.GetNumberField("ID");

	return game;
}

void USaveAndLoadSubsystem::SortSaveGames(TArray<FVergjornSaveGame>& saves)
{
	//Bubble sort algorithm
	auto& unsorted_array = saves;
	int n = saves.Num();
	int i, j;
	bool swapped;
	for (i = 0; i < n - 1; i++) {
		swapped = false;
		for (j = 0; j < n - i - 1; j++) {
			if (unsorted_array[j].m_DateTime < unsorted_array[j + 1].m_DateTime) {
				unsorted_array.Swap(j, j + 1);
				swapped = true;
			}
		}
		if (swapped == false)
			break;
	}
}
