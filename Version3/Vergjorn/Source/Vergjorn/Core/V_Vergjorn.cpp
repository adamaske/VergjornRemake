// Fill out your copyright notice in the Description page of Project Settings.


#include "V_Vergjorn.h"
#include "Vergjorn.h"


UV_Vergjorn::UV_Vergjorn()
{

}

void UV_Vergjorn::Init()
{
	GEngine->AddOnScreenDebugMessage(VERGJORN_INIT, 5, FColor::Green, FString::Printf(TEXT("Vergjorn Init")));

	//LOAD VERGJORN FILES
	mSaveFiles = LoadVergjornSaves();
	//Get the newgame file
	mNewGameSaveFile = LoadNewGameFile();
}

void UV_Vergjorn::Shutdown()
{
	GEngine->AddOnScreenDebugMessage(VERGJORN_SHUTDOWN, 5, FColor::Green, FString::Printf(TEXT("Vergjorn Shutdown")));
}

#pragma region Saving And Loading
TArray<FJsonObject> UV_Vergjorn::LoadVergjornSaves()
{
	//Load all save files
	FString file_prefix = "VergjornSave";
	FString file_suffix = ".verg";

	//Cache the loaded json objects
	TArray<FJsonObject> loadedJsonObjects;
	for (int file_index = 0; file_index < 100; file_index++) {
		//----FILE PATHS-----
		FString file_name = file_prefix;
		file_name.Append("_");
		file_name.Append(FString::FromInt(file_index));
		file_name.Append(file_suffix);

		FString file_path = FPaths::ProjectContentDir();
		file_path.Append(file_name);
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("Filepath : %s"), *file_path));

		FString loaded = "";
		FFileHelper::LoadFileToString(loaded, *file_path);//Load the file to the string

		//----------JSON DESERIALIZATION---------------
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(loaded);//reader
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);//empty object
		bool success = FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid();

		if (!success) {
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Unsuccessful Json Deserialization : %s"), *file_path));
			continue;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Successfull Json Deserialization : %s"), *file_name));

		loadedJsonObjects.Add(*JsonObject);//add the value
	}

	//Store the save file amount

	return loadedJsonObjects;
}
bool UV_Vergjorn::SaveVergjornFile(FJsonObject* obj)//Find the first free save slot
{
	FString file_prefix = "VergjornSave";
	FString file_suffix = ".verg";
	int file_index = 0;
	for (int index = 0; index < 100; index++)
	{
		FString file_name = file_prefix;
		file_name.Append("_");
		file_name.Append(FString::FromInt(index));
		file_name.Append(file_suffix);

		FString file_path = FPaths::ProjectContentDir();
		file_path.Append(file_name);

		FString loaded = "";
		FFileHelper::LoadFileToString(loaded, *file_path);//Load the file to the string

		bool success = loaded == "";
		if (success) {//No file at this index, save at it
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("First free save index : %d"), index));
			file_index = index;
			break;
		}

		if (index == 99) {//If every index can be loaded, return false
			return false;
		}
	}
	//A free slot was found, loading that
	return SaveVergjornFile(obj, file_index);

}
bool UV_Vergjorn::SaveVergjornFile(FJsonObject* obj, int file_index)
{
	FString file_prefix = "VergjornSave";
	FString file_suffix = ".verg";
	FString file_name = file_prefix;
	file_name.Append("_");
	file_name.Append(FString::FromInt(file_index));
	file_name.Append(file_suffix);

	FString file_path = FPaths::ProjectContentDir();
	file_path.Append(file_name);

	TSharedRef<FJsonObject> json = MakeShareable<FJsonObject>(obj);
	FString jsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&jsonString);
	FJsonSerializer::Serialize(json, Writer);
	FFileHelper::SaveStringToFile(jsonString, *file_path);

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Saved JSON object at : %s"), *file_path));
	return true;
}


TArray<FJsonObject*> UV_Vergjorn::GetSaveFiles()//Creates pointers to the json objects
{
	TArray<FJsonObject*> files;
	for (int i = 0; i < mSaveFiles.Num(); i++)
	{
		files.Add(&mSaveFiles[i]);
	}

	return files;
}

FJsonObject UV_Vergjorn::LoadNewGameFile() {

	//----FILE PATHS-----
	FString file_name = "NewGameSave.verg";

	FString file_path = FPaths::ProjectContentDir();
	file_path.Append(file_name);
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("Filepath : %s"), *file_path));

	FString loaded = "";
	FFileHelper::LoadFileToString(loaded, *file_path);//Load the file to the string

	//----------JSON DESERIALIZATION---------------
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(loaded);//reader
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);//empty object
	bool success = FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid();

	if (!success) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Unsuccessful New Game Loading at : %s"), *file_path));
		FJsonObject obj;
		return obj;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Successfull Json Deserialization : %s"), *file_name));
	return *JsonObject.Get();
}
		
//Returns the New-game save file
FJsonObject UV_Vergjorn::GetNewGameFile() {
	return mNewGameSaveFile;
}


#pragma endregion
