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

	//Depedent on other subsystems to be already launched
	Collection.InitializeDependency<UVergjornSubsystem>();


}

void USaveAndLoadSubsystem::Deinitialize()
{
}

void USaveAndLoadSubsystem::Save(FVergjornSaveGame save, bool& outSuccess)
{
	auto obj = SaveGameToJson(save);
	auto path = GetSaveGameFilepath(save);

	bool success = false;
	FString info = "";

	WriteJson(path, obj, success, info);
	if (!success) {
		outSuccess = success;
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("SL_SS : Save failed"), *info));
		return;
	}
	outSuccess = true;
}

void USaveAndLoadSubsystem::Quicksave(bool& outSuccess, FString& info)
{
	auto GI = GetGameInstance();
	auto vergjorn = GI->GetSubsystem<UVergjornSubsystem>();
	if (!vergjorn) {
		outSuccess = false;
		info = "No Vergjorn Subsystem!";
		return;
	}

	FVergjornSaveGame save = vergjorn->GetWorldAsSaveGame();
	Save(save, outSuccess);
}
void USaveAndLoadSubsystem::Load(FVergjornSaveGame save, bool& outSuccess, FString& info)
{
	auto GI = GetGameInstance();
	auto vergjorn = GI->GetSubsystem<UVergjornSubsystem>();
	if (!vergjorn) {
		outSuccess = false;
		info = "No Vergjorn Subsystem!";
		return;
	}

	vergjorn->SetWorldAsSaveGame(save);
}

void USaveAndLoadSubsystem::Quickload(bool& outSuccess, FString& info)
{
	SortSaveGames(m_SavesArray);

	auto toLoad = m_SavesArray[0];

	Load(toLoad, outSuccess, info);
}

int USaveAndLoadSubsystem::LoadAndCacheSaves()
{
	auto files = GetFilesInSaveGameFolder();
	auto folder = GetSaveGameFolderPath();
	int amount = files.Num();

	bool success = false;
	FString info = "";

	//Cache json objects read from file
	m_CachedJsonSaveGames.Empty();
	for (int i = 0; i < amount; i++) {
		auto path = FPaths::Combine(folder, files[i]);
		auto json = ReadJson(path, success, info);
		if (!success) {

			continue;
		}
		m_CachedJsonSaveGames.Add(json);
	}
	files.Empty();

	//Turn cached json objects to save games
	TArray<FVergjornSaveGame> loadedSaveGames;
	for (int i = 0; i < m_CachedJsonSaveGames.Num(); i++) {
		auto save = JsonToSaveGame(m_CachedJsonSaveGames[i]);
		loadedSaveGames.Add(save);
	}

	m_SavesArray.Empty();
	//Add to Saves
	for (auto save : loadedSaveGames) {
		m_SavesArray.Add(save);
		m_SavesMap.Add(save.m_ID, save);
	}
	loadedSaveGames.Empty();

	SortSaveGames(m_SavesArray);
	return m_SavesArray.Num();
}

TArray<FVergjornSaveGame> USaveAndLoadSubsystem::GetSaves()
{
	return m_SavesArray;
}

void USaveAndLoadSubsystem::CreateNewGame(FVergjornSaveGame save)
{
	//Idk what the intended use for this function was?
	//Maybe to populate the save file for a new game
	//However this should be a predefined save file
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

TSharedPtr<FJsonObject> USaveAndLoadSubsystem::SaveGameToJson(FVergjornSaveGame save)
{
	TSharedPtr<FJsonObject> json = MakeShareable<FJsonObject>(new FJsonObject);
	auto obj = json.Get();
	obj->SetStringField("Title", save.m_Title);
	obj->SetStringField("ClanName", save.m_ClanName);
	obj->SetStringField("Datetime", save.m_DateTime.ToString());
	obj->SetNumberField("ID", save.m_ID);
	return json;
}

FVergjornSaveGame USaveAndLoadSubsystem::JsonToSaveGame(TSharedPtr<FJsonObject> object)
{	
	auto json = object.Get();

	FVergjornSaveGame game;
	game.m_Title = json->GetStringField("Title");
	game.m_ClanName = json->GetStringField("ClanName");
	if (!FDateTime::Parse(json->GetStringField("Datetime"), game.m_DateTime)) 
	{
		game.m_DateTime = FDateTime::Now();
	}
	game.m_ID = json->GetNumberField("ID");

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

FString USaveAndLoadSubsystem::GetSaveGameFilepath(FVergjornSaveGame save)
{
	FString filepath = m_SaveGameFolderPath;
	filepath.Append(m_SaveFilePrefix);
	filepath.Append("-");
	filepath.Append(save.m_ClanName);
	filepath.Append("-");
	filepath.Append(save.m_DateTime.ToString());
	filepath.Append(m_SaveFileSuffix);
	return filepath;
}

FString USaveAndLoadSubsystem::GetSaveGameFolderPath()
{
	return m_SaveGameFolderPath;
}

TArray<FString> USaveAndLoadSubsystem::GetFilesInSaveGameFolder()
{
	TArray<FString> fileNames;//Cache the found filenames

	FFileManagerGeneric fileManager;
	fileManager.SetSandboxEnabled(true);// don't ask why, I don't know :P

	FString wildcard("*");//Find every file with the suffix
	wildcard.Append(m_SaveFileSuffix);

	FString search_path(FPaths::Combine(GetSaveGameFolderPath(), *wildcard));
	fileManager.FindFiles(fileNames, *search_path, true, false);

	return fileNames;
}

TSharedPtr<FJsonObject> USaveAndLoadSubsystem::ReadJson(FString path, bool& outSuccess, FString& outMessage)
{
	FString loaded = "";
	FFileHelper::LoadFileToString(loaded, *path);//Load the file to the string

	//----------JSON DESERIALIZATION---------------
	TSharedPtr<FJsonObject> json;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<TCHAR>::Create(loaded), json))
	{
		outSuccess = false;
		outMessage = "Deserialization failed!";
		return nullptr;
	}

	if (!json.IsValid())
	{
		outSuccess = false;
		outMessage = "JsonObject invalid!";
		return nullptr;
	}
	outSuccess = true;
	return json;
}

void USaveAndLoadSubsystem::WriteJson(FString path, TSharedPtr<FJsonObject> object, bool& outSuccess, FString& outMessage)
{
	FString json;//Put serialized json object into jsonString
	if (!FJsonSerializer::Serialize(object.ToSharedRef(), TJsonWriterFactory<>::Create(&json, 0))) 
	{
		outSuccess = false;
		outMessage = FString::Printf(TEXT("SL_SS : Could not write Json!"));
		return;
	}

	//Then write to file
	if (!FFileHelper::SaveStringToFile(json, *path)) 
	{
		outSuccess = false;
		outMessage = FString::Printf(TEXT("SL_SS : Could not wsave string to file!"));
		return;
	}

	outSuccess = true;
}
