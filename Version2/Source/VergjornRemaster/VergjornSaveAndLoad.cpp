// Fill out your copyright notice in the Description page of Project Settings.


#include "VergjornSaveAndLoad.h"
#include "JsonObjectConverter.h"
#include "VergjornSaveGame.h"
#include <string>
#include <fstream>

#include "Kismet/GameplayStatics.h"
#include <Serialization/JsonSerializer.h>
static std::string VergjornSavePrefix = "VergjornSave";

VergjornSaveAndLoad::VergjornSaveAndLoad()
{
}

VergjornSaveAndLoad::~VergjornSaveAndLoad()
{
}

void VergjornSaveAndLoad::Save(UVergjornSaveGame g, FString name, int index)
{

	//Saving
	auto save = Cast<UVergjornSaveGame>(UGameplayStatics::CreateSaveGameObject(UVergjornSaveGame::StaticClass()));
	if (!save) {
		//Did not manage to create a vergjorn save game
	}
	FAsyncSaveGameToSlotDelegate savedDelegate;
	savedDelegate.BindRaw(this, &VergjornSaveAndLoad::CompletedSave);

	save->MapName = "Volugrimar";

	UGameplayStatics::AsyncSaveGameToSlot(save, name, index, savedDelegate);

}

void VergjornSaveAndLoad::CompletedSave(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
}

UVergjornSaveGame* VergjornSaveAndLoad::Load(FString name, int index)
{
	auto save = Cast<UVergjornSaveGame>(UGameplayStatics::LoadGameFromSlot(name, index));
	if (save) {
		return save;
	}
	return nullptr;
}

TArray<class UVergjornSaveGame*> VergjornSaveAndLoad::LoadAllVergjornSaves()
{
	return TArray<class UVergjornSaveGame*>();
}



std::string VergjornSaveAndLoad::GetSaveFileName(int index)
{
	std::string base = VergjornSavePrefix;
	base += "_";
	base += "0";
	base += ".save";
	return base;
}
