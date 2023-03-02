// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

 //Contains all data for loading a vergjorn game
struct VergjornConfig {
	int mMostRecentSaveGame = 0;

	bool bEmptyConfig = false;
};


class VERGJORNREMASTER_API VergjornSaveAndLoad
{
public:
	VergjornSaveAndLoad();
	~VergjornSaveAndLoad();
	
	static void Save(class UVergjornSaveGame* g, FString name, int index);
	void CompletedSave(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	static UVergjornSaveGame* Load(FString name, int index);


	static std::string GetSaveFileName(int index);
	static VergjornConfig LoadVergjornConfigFile(std::string fileName);
	static void CreateVergjornConfigFile(std::string fileName, VergjornConfig con);
private:
};
