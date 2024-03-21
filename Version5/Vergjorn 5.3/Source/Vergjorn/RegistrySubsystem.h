// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RegistrySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class VERGJORN_API URegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
    // Begin USubsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    virtual void Deinitialize() override;
    // End USubsystem
};
