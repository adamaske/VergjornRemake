// Copyright Epic Games, Inc. All Rights Reserved.


#include "VergjornGameModeBase.h"
#include "Vergjorn_Instance.h"
#include "Blueprint/UserWidget.h"

void AVergjornGameModeBase::BeginPlay()
{
	Super::BeginPlay();

    //Register game mode at gameinstance
    auto instance =  Cast<UVergjorn_Instance>(GetGameInstance());
    if (instance) {
        instance->RegisterGameMode(this);
    }

}

