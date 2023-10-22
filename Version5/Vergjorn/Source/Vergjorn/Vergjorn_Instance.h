// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Vergjorn_Instance.generated.h"

class AVergjornGameModeBase;

/**
 * 
 */
UCLASS()
class VERGJORN_API UVergjorn_Instance : public UGameInstance
{
	GENERATED_BODY()
public:
	UVergjorn_Instance();

	virtual void Init() override;

	virtual void Shutdown() override;

	//----VERGJORN------
	//UFUNCTION(BlueprintCallable)
	//UV_Vergjorn* GetVergjorn();

public:

	void RegisterGameMode(AVergjornGameModeBase* gm);
	//UFUNCTION(BlueprintCallable)
	//static UVergjorn_Instance* GetInstance() {
	//	return Instance;
	//}
	//static UVergjorn_Instance* Instance;
private:
	//TObjectPtr<UV_Vergjorn> mVergjorn = nullptr;




};
