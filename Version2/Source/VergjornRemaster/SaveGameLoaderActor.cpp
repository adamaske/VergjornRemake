// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameLoaderActor.h"
#include "VergjornSaveGame.h"
#include "VergjornGameInstance.h"

// Sets default values
ASaveGameLoaderActor::ASaveGameLoaderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Loader Actor Created!")));

}

// Called when the game starts or when spawned
void ASaveGameLoaderActor::BeginPlay()
{
	Super::BeginPlay();

	Cast<UVergjornGameInstance>(GetGameInstance())->SetLoaderActor(this);

	if (bLoadActiveSaveFromGameInstance) {
		bool loaded = LoadSave(Cast<UVergjornGameInstance>(GetGameInstance())->GetActiveSaveGame());
		if (!loaded) {
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Failed to load save in begin play")));
		}
	}
}

// Called every frame
void ASaveGameLoaderActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ASaveGameLoaderActor::LoadSave(UVergjornSaveGame* save)
{
	//Apply to world here
	if (!save) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Loader : LoadSave save parameter is null!")));
		return false;
	}

	//Read what type actors should exist, terrain, structures, humans, animals, resources, etc

	//Spawn the correct blueprints and 

	//We can use a map maybe hmm
	return false;
}

