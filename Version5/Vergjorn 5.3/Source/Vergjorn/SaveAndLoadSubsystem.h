// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveAndLoadSubsystem.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FVergjornSaveGame {
	GENERATED_BODY()
public:
	FVergjornSaveGame() {

	};
	FVergjornSaveGame(FString title, FString clan_name, FDateTime datetime, int id) {

		m_Title = title;
		m_ClanName = clan_name;
		m_DateTime = datetime;
		m_ID = id;
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	FString m_Title = "Save_00";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	FString m_ClanName = "Adam's Clan";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	FDateTime m_DateTime = FDateTime::Now();

	int m_ID = 0;
};

UCLASS(DisplayName = "SaveAndLoad Subsystem")
class VERGJORN_API USaveAndLoadSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    // Begin USubsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    virtual void Deinitialize() override;
    // End USubsystem

	//What does this subsystem do?
	//Load and cache all saves found in directory

	//
	UFUNCTION(BlueprintCallable, Category = UI)
	void Save(FVergjornSaveGame save);

	void Load();

	//Save the current active world
	void Quicksave();
	//Load the latest save file
	void Quickload();

	//Cache all vergjorn saves
	UFUNCTION(BlueprintCallable, Category = UI)
	int LoadAndCacheSaves();

	UFUNCTION(BlueprintCallable, Category = UI)
	TArray<FVergjornSaveGame> GetSaves();
	//
	UFUNCTION(BlueprintCallable, Category = UI)
	void CreateNewGame(FVergjornSaveGame save);

	UFUNCTION(BlueprintCallable)
	void SetActiveSaveGame(int ID);

	//Helpers

	FJsonObject SaveGameToJson(FVergjornSaveGame save);
	FVergjornSaveGame JsonToSaveGame(FJsonObject object);

	void SortSaveGames(TArray<FVergjornSaveGame>& saves);
private:
    // All my variables

	TArray<FVergjornSaveGame> m_SavesArray;
	TMap<int, FVergjornSaveGame> m_SavesMap;
	
	int m_ActiveSaveKey = 0; 
	FVergjornSaveGame m_ActiveSaveGame = FVergjornSaveGame{ "NewGame", "None", FDateTime::Now(), 0 };


	FString m_SaveFolderFilePath = "C:/Vergjorn/Saves/";

	FString mSaveFile_Prefix = "Vergjorn";
	FString mSaveFile_Suffix = ".save";

};
