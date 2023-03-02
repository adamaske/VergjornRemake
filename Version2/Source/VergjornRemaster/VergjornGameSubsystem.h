// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VergjornGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class VERGJORNREMASTER_API UVergjornGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
    // Begin USubsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
   
};
