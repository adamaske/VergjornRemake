// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VergjornGameModeBase.generated.h"


USTRUCT(BlueprintType)
struct FVergjorn_Game_Mode {
    GENERATED_BODY()
public:


};

/**
 * 
 */
UCLASS()
class VERGJORN_API AVergjornGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	//Keep track of what game mode we're in

protected:
    /** Called when the game starts. */
    virtual void BeginPlay() override;
private:


public:
    /** Remove the current menu widget and create a new one from the specified class, if provided. */
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
    void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

private:
    /** The widget class we will use as our menu when the game starts. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game", meta = (AllowPrivateAccess))
    TSubclassOf<UUserWidget> StartingWidgetClass;

    /** The widget instance that we are using as our menu. */
    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentWidget;

};
