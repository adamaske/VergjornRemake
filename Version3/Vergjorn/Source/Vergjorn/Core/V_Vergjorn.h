// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "V_Vergjorn.generated.h"

/**
 * 
 */
//This class is the main Vergjorn class
UCLASS(config = Game, BlueprintType, Blueprintable)
class VERGJORN_API UV_Vergjorn : public UObject
{
	GENERATED_BODY()
public:
	UV_Vergjorn();

	void Init();
	void Shutdown();

#pragma region Saving and Loading
	//Loads all vergjorn save files 0-99
	TArray<FJsonObject> LoadVergjornSaves();
	//Save vergjorn file at unspecifed index
	bool SaveVergjornFile(FJsonObject* obj);
	//Save at specific index
	bool SaveVergjornFile(FJsonObject* obj, int file_index);
	//Returns pointers to all loaded save files
	TArray<FJsonObject*> GetSaveFiles();

	//Set the active world to be the New-game file
	FJsonObject LoadNewGameFile();
	//Returns a copy of the New-game save file
	FJsonObject GetNewGameFile();
#pragma endregion
private:

	TArray<FJsonObject> mSaveFiles;

	FJsonObject mNewGameSaveFile;

};
