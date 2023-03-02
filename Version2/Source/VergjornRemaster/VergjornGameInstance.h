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

    UFUNCTION(BlueprintCallable)
        void SetActiveMapSaveGame(UVergjornSaveGame* save);
    UFUNCTION(BlueprintCallable)
        UVergjornSaveGame* GetActiveSaveGame();
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int mNewestSaveIndex;
    UFUNCTION(BlueprintCallable)
        void SaveGameIncremented(UVergjornSaveGame* save, FString name, int userIndex);

    UFUNCTION(BlueprintCallable)
    void Save();
private:
    // All my variables
    std::shared_ptr<int> mActiveSave;

    //Take ownership of it
    std::shared_ptr<UVergjornSaveGame> mActiveVergjornSaveGame;
};
