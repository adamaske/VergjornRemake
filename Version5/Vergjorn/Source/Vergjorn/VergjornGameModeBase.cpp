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

	ChangeMenuWidget(StartingWidgetClass);
}

void AVergjornGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{

    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
}
