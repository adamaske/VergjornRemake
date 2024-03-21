// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveAndLoadSubsystem.generated.h"


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


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadedSaveFiles);


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
	UFUNCTION(BlueprintCallable, Category = SaveAndLoad)
	void Save(FVergjornSaveGame save, bool& success);
	//Save the current active world
	UFUNCTION(BlueprintCallable, Category = SaveAndLoad)
	void Quicksave(bool& outSuccess, FString& info);

	UFUNCTION(BlueprintCallable, Category = SaveAndLoad)
	void Load(FVergjornSaveGame save, bool& outSuccess, FString& info);

	//Load the latest save file
	UFUNCTION(BlueprintCallable, Category = SaveAndLoad)
	void Quickload(bool& outSuccess, FString& info);


	//Loads all save files
	//TODO 
	//- Dont reload every save file everytime
	//- GUID system for save files
	//
	UFUNCTION(BlueprintCallable, Category = SaveAndLoad)
	int LoadAndCacheSaves();

	UFUNCTION(BlueprintCallable, Category = SaveAndLoad)
	TArray<FVergjornSaveGame> GetSaves();
	//
	UFUNCTION(BlueprintCallable, Category = SaveAndLoad)
	void CreateNewGame(FVergjornSaveGame save);

	UFUNCTION(BlueprintCallable, Category = SaveAndLoad)
	void SetActiveSaveGame(int ID);

	//Helpers
	
	TSharedPtr<FJsonObject> SaveGameToJson(FVergjornSaveGame save);
	FVergjornSaveGame JsonToSaveGame(TSharedPtr<FJsonObject> object);

	void SortSaveGames(TArray<FVergjornSaveGame>& saves);
	
	UPROPERTY(BlueprintAssignable, Category = SaveAndLoad)
	FLoadedSaveFiles m_LoadedSaveFilesDelegate;

private:
    // All my variables

	TArray<FVergjornSaveGame> m_SavesArray;
	TMap<int, FVergjornSaveGame> m_SavesMap;
	
	TArray<TSharedPtr<FJsonObject>> m_CachedJsonSaveGames;

	int m_ActiveSaveKey = 0; 
	FVergjornSaveGame m_ActiveSaveGame = FVergjornSaveGame{ "NewGame", "None", FDateTime::Now(), 0 };

	//TODO 
	//This should be a relative path
	FString m_SaveGameFolderPath = "C:/Vergjorn/Saves/";
	FString m_SaveFilePrefix = "Vergjorn";
	FString m_SaveFileSuffix = ".save";

	FString GetSaveGameFilepath(FVergjornSaveGame save);
	FString GetSaveGameFolderPath();
	TArray<FString> GetFilesInSaveGameFolder();

	TSharedPtr<FJsonObject> ReadJson(FString path, bool& outSuccess, FString& outMessage);
	void WriteJson(FString path, TSharedPtr<FJsonObject> object, bool& outSuccess, FString& outMessage);
};
