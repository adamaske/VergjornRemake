// Fill out your copyright notice in the Description page of Project Settings.


#include "VergjornMapGameMode.h"
#include "VergjornSaveGame.h"
#include "VergjornGameInstance.h"
void AVergjornMapGameMode::StartPlay()
{
	//Super::StartPlay();

	//Load what save game to use
	auto  vergjorn = Cast<UVergjornGameInstance>(GetGameInstance());
	if (!vergjorn) {
		//Error
	}

	auto save = vergjorn->GetActiveSaveGame();
	if (!save) {
		//Error
	}
}
