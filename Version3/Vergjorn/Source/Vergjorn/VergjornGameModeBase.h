// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VergjornGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VERGJORN_API AVergjornGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AVergjornGameModeBase();
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
