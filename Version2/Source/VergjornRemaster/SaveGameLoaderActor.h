// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveGameLoaderActor.generated.h"

UCLASS()
class VERGJORNREMASTER_API ASaveGameLoaderActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveGameLoaderActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool LoadSave(class UVergjornSaveGame* save);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLoadActiveSaveFromGameInstance = true;
	//We need some blueprints for what type of actors we should spawn depending on what structure we get from the save file
	//TMap<std::string, TSubclassOf<AActor>> mActorBlueprintsMap;
	//TMap<Agent, int> mSecondMap;
	////If we wor example send a aworker down, then what do we associate it with
	//template<typename T>
	//bool LoadActor(T clas);
};
