// Copyright Epic Games, Inc. All Rights Reserved.

#include "Vergjorn.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Vergjorn, "Vergjorn" );

#include "V_SaveAndLoad.h"
Vergjorn::Vergjorn()
{

}

Vergjorn::~Vergjorn()
{
}

void Vergjorn::Init()
{
	GEngine->AddOnScreenDebugMessage(VERGJORN_INIT, 5, FColor::Green, FString::Printf(TEXT("Vergjorn Init")));

//LOAD VERGJORN FILES
	mSaveAndLoad = new V_SaveAndLoad();//Is this needed ? prob not
	TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject);
	json->SetStringField("GUID", "Testing");
	V_SaveAndLoad::SaveVergjornFile(*json);

	mSaveFiles = V_SaveAndLoad::LoadAllVergjornSaves();
	


}

void Vergjorn::Shutdown()
{
	GEngine->AddOnScreenDebugMessage(VERGJORN_SHUTDOWN, 5, FColor::Green, FString::Printf(TEXT("Vergjorn Shutdown")));

}

TArray<FJsonObject> Vergjorn::GetSaveFiles()
{
	TArray<FJsonObject> objs;
	for (int i = 0; i < mSaveFiles.size(); i++)
	{
		objs.Add(mSaveFiles[i]);
	}
	return objs;
}
