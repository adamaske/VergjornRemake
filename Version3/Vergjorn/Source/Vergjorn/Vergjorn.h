// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


//Primary class for vergjorn
class V_SaveAndLoad;
class Vergjorn {
public:
	Vergjorn();
	~Vergjorn();

	void Init();
	void Shutdown();

	TArray<FJsonObject> GetSaveFiles();
private:
	V_SaveAndLoad* mSaveAndLoad;//For saving and loading
	std::vector<FJsonObject> mSaveFiles = {};
};

//DEBUGGING KEYS
#define GI_INIT 0
#define GI_SHUTDOWN 1

#define VERGJORN_INIT 2
#define VERGJORN_SHUTDOWN 3 