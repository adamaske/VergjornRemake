// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

 //Contains all data for loading a vergjorn game

struct VergjornSave {
public:
	std::string mSaveName;
	int mSaveIndex = 0;
};

class VERGJORNREMASTER_API VergjornSaveAndLoad
{
public:
	VergjornSaveAndLoad();
	~VergjornSaveAndLoad();

	void Save();
	void Load();

	void LoadAllSaves();
	TArray<VergjornSave> GetAllVergjornSaves();

	std::string GetSaveFileName(int index);
private:
	std::string mSavePrefix = "VergjornSave";
};
