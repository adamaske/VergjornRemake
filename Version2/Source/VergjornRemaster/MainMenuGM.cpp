// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGM.h"
#include "VergjornGameInstance.h"
#include "VergjornSaveGame.h"

AMainMenuGM::AMainMenuGM()
{
	//We want to load every vergjorn save file

}

AMainMenuGM::~AMainMenuGM()
{
}

//void AMainMenuGM::StartPlay()
//{
//	LoadSaves();
//}

void AMainMenuGM::LoadSaves()
{
	auto saves = Cast<UVergjornGameInstance>(GetGameInstance())->LoadVergjornSaves();
	for (auto save : saves) {
		mLoadedSaves.Add(save);
	}
}
