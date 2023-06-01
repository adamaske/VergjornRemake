// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class VERGJORN_API V_SaveAndLoad
{
public:
	V_SaveAndLoad();
	~V_SaveAndLoad();

	static std::vector<FJsonObject> LoadAllVergjornSaves();
	//void SaveVergjornState();
	
	//Save with unspecified index
	static bool SaveVergjornFile(FJsonObject obj);
	static bool SaveVergjornFile(FJsonObject obj, int file_index);

private:
	int mAmoutOfSaveFiles = 0;

	FString mSaveFile_Prefix = "VergjornSave";
	FString mSaveFile_Suffix = ".verg";

};
