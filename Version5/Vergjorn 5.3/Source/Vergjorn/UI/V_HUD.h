// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "V_HUD.generated.h"

/**
 * 
 */
UCLASS()
class VERGJORN_API AV_HUD : public AHUD
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent)
	void NextScreen();

	//TMap<int, m_Screens
	
	
};
