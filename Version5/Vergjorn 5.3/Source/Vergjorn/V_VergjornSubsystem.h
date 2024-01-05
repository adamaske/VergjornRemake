// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "V_VergjornSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class VERGJORN_API UV_VergjornSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
    // Begin USubsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    virtual void Deinitialize() override;
    // End USubsystem

    UFUNCTION(BlueprintCallable)
    void Test();

private:
    // All my variables
};
