// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Vergjorn_Instance.generated.h"

class AVergjornGameModeBase;
struct FVergjornSaveGame;
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

public:
	void RegisterGameMode(AVergjornGameModeBase* gm);

	//----- SAVE GAMES & LOADING -------

	UFUNCTION(BlueprintCallable)
	void NewGame();

	UFUNCTION(BlueprintCallable)
	void Continue();

	UFUNCTION(BlueprintCallable)
	void LoadGame(FVergjornSaveGame save);

	UFUNCTION(BlueprintCallable)
	void ExitVergjorn();
private:
	
};
