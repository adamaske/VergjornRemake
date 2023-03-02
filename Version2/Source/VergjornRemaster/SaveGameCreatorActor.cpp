// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameCreatorActor.h"
#include "VergjornGameInstance.h"
#include "VergjornSaveGame.h"
// Sets default values
ASaveGameCreatorActor::ASaveGameCreatorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Creator Actor Created!")));
}

// Called when the game starts or when spawned
void ASaveGameCreatorActor::BeginPlay()
{
	Super::BeginPlay();
	
	Cast<UVergjornGameInstance>(GetGameInstance())->SetCreatorActor(this);
}

// Called every frame
void ASaveGameCreatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

 void ASaveGameCreatorActor::GetVergjornSaveGameFromWorld(UVergjornSaveGame* save)
{
	//Read world here and apply it to the save game

	
}

