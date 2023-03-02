// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VergjornGameMode.h"
#include "VergjornMapGameMode.generated.h"

/**
 * 
 */
UCLASS()
class VERGJORNREMASTER_API AVergjornMapGameMode : public AVergjornGameMode
{
	GENERATED_BODY()
public:

		virtual void StartPlay() override;

};
