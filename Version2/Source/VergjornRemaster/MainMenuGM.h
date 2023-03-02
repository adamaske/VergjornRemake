// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VergjornRemasterGameMode.h"
#include "MainMenuGM.generated.h"

/**
 * 
 */
UCLASS()
class VERGJORNREMASTER_API AMainMenuGM : public AVergjornRemasterGameMode
{
	GENERATED_BODY()
public:
	AMainMenuGM();
	~AMainMenuGM();

	void LoadSaves();
	UPROPERTY()
	TArray<class UVergjornSaveGame*> mLoadedSaves;
};
