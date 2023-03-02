// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VergjornGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VERGJORNREMASTER_API UVergjornGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
    //virtual void Init() override;
    // End USubsystem
    void VergjornStarted();

    std::vector<std::shared_ptr<class UVergjornSaveGame>> LoadVergjornSaves();

    
private:
    // All my variables
    std::shared_ptr<int> mActiveSave;
};
