// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

 //Contains all data for loading a vergjorn game

class VERGJORNREMASTER_API VergjornSaveAndLoad
{
public:
	VergjornSaveAndLoad();
	~VergjornSaveAndLoad();
	
	void Save(class UVergjornSaveGame g, FString name, int index);
	void CompletedSave(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	UVergjornSaveGame* Load(FString name, int index);

	TArray<UVergjornSaveGame*> LoadAllVergjornSaves();

	static std::string GetSaveFileName(int index);
private:
};
