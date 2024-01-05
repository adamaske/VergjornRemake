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

private:


};
