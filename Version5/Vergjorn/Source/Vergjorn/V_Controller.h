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

UENUM(BlueprintType)
enum E_V_InputType {
	IT_KeyboardMouse UMETA(DisplayName = "KeyboardMouse"),
	IT_Gamepad UMETA(DisplayName = "Gamepad"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputTypeSwitch, E_V_InputType, new_type);
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TEnumAsByte<E_V_InputType> m_InputType = E_V_InputType::IT_Gamepad;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FInputTypeSwitch m_InputTypeSwitched;
public:
	UFUNCTION(BlueprintCallable)
	void SwitchInputMode(E_V_InputType type);
};
