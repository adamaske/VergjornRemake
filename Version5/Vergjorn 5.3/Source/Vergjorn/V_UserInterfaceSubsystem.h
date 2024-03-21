// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "V_UserInterfaceSubsystem.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "DECAPERATED Subsystem")
class VERGJORN_API UV_UserInterfaceSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    // Begin USubsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    virtual void Deinitialize() override;
    // End USubsystem
    

private:
    // All my variables
	
};
