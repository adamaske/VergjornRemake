// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "V_GameInstance.generated.h"

/**
 * 
 */

class UV_Vergjorn;
class V_Entity;
UCLASS()
class VERGJORN_API UV_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UV_GameInstance();

	virtual void Init() override;

	virtual void Shutdown() override;

	//----VERGJORN------
	UFUNCTION(BlueprintCallable)
		UV_Vergjorn* GetVergjorn();

private:
	TObjectPtr<UV_Vergjorn> mVergjorn = nullptr;
};
