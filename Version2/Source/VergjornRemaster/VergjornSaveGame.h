// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VergjornSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class VERGJORNREMASTER_API UVergjornSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UVergjornSaveGame();
	UPROPERTY(VisibleAnywhere, Category = Basic, BlueprintReadWrite)
		FString PlayerName = "Adam";

	UPROPERTY(VisibleAnywhere, Category = Basic, BlueprintReadWrite)
		FString MapName = "Volgurimar";

	UPROPERTY(VisibleAnywhere, Category = Basic, BleuprintReadWrite)
	TArray<FWorker> Workers;
};

USTRUCT()
struct FWorker {

};
