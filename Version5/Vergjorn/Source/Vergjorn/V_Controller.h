// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "V_Controller.generated.h"

/**
 * 
 */

class AV_HUD;
class AEntitySelector;
class AV_Player;

UCLASS()
class VERGJORN_API AV_Controller : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void OnPossess(APawn* pawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

private:
	
private:
	//Player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	AV_Player* mPlayer;

	
};
