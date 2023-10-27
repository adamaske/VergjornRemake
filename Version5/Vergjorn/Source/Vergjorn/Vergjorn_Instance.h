// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Vergjorn_Instance.generated.h"

class AVergjornGameModeBase;

USTRUCT(BlueprintType)
struct FVergjorn_SaveGame {
	GENERATED_BODY()
public:
	FVergjorn_SaveGame() {

	};
	FVergjorn_SaveGame(FString title, FString clan_name, FDateTime datetime, int id) {

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
/**
 * 
 */
UCLASS()
class VERGJORN_API UVergjorn_Instance : public UGameInstance
{
	GENERATED_BODY()
public:
	UVergjorn_Instance();

	virtual void Init() override;

	virtual void Shutdown() override;

	//----VERGJORN------
	//UFUNCTION(BlueprintCallable)
	//UV_Vergjorn* GetVergjorn();

private:
	//Cache all vergjorn saves
	void Load_Saves();

public:
	void RegisterGameMode(AVergjornGameModeBase* gm);
	//UFUNCTION(BlueprintCallable)
	//static UVergjorn_Instance* GetInstance() {
	//	return Instance;
	//}
	//static UVergjorn_Instance* Instance;

	//----- SAVE GAMES & LOADING -------

	UFUNCTION(BlueprintCallable)
	void NewGame();

	UFUNCTION(BlueprintCallable)
	void Continue();

	UFUNCTION(BlueprintCallable)
	void LoadGame(FVergjorn_SaveGame save);

	UFUNCTION(BlueprintCallable)
	TArray<FVergjorn_SaveGame> Get_SaveGames();


	UFUNCTION(BlueprintCallable)
	void ExitVergjorn();
private:
	//TObjectPtr<UV_Vergjorn> mVergjorn = nullptr;

	TArray<FVergjorn_SaveGame> m_Loaded_GameSaves;

	//Helper function
	TArray<FVergjorn_SaveGame> Sort_Saves(TArray<FVergjorn_SaveGame> files);

};
