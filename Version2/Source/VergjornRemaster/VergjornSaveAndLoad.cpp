// Fill out your copyright notice in the Description page of Project Settings.


#include "VergjornSaveAndLoad.h"

VergjornSaveAndLoad::VergjornSaveAndLoad()
{
}

VergjornSaveAndLoad::~VergjornSaveAndLoad()
{
}

void VergjornSaveAndLoad::Save()
{
	//Saving
}

void VergjornSaveAndLoad::Load() {
	//Loading
}

void VergjornSaveAndLoad::LoadAllSaves()
{
	//Start at save index 0
	int saveIndex = 0;
	std::string fileDir = std::string(TCHAR_TO_UTF8(*FPaths::GetProjectFilePath())) + "/"; ;//Get Save Dir
	std::string fileName = GetSaveFileName(saveIndex);
	
	//While we can find the file with the idnex we load more fiels
	while (1) {
		std::string file = fileDir + GetSaveFileName(saveIndex);

		saveIndex++;
	}


}

TArray<VergjornSave> VergjornSaveAndLoad::GetAllVergjornSaves()
{
	return TArray<VergjornSave>();
}

std::string VergjornSaveAndLoad::GetSaveFileName(int index)
{
	std::string base = mSavePrefix;
	base += "_";
	base += "0";
	base += ".save";
	return base;
}
