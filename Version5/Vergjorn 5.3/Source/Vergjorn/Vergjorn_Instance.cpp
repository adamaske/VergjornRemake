// Fill out your copyright notice in the Description page of Project Settings.


#include "Vergjorn_Instance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveAndLoadSubsystem.h"
UVergjorn_Instance::UVergjorn_Instance()
{
}

void UVergjorn_Instance::Init()
{
	Super::Init();
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("GI Init")));
	
	//Create Vergjorn
	//mVergjorn = NewObject<UV_Vergjorn>();

	//Vergjorn
	//mVergjorn->Init();

}

void UVergjorn_Instance::Shutdown()
{
	Super::Shutdown();
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("GI Shutdown")));


	//mVergjorn->Shutdown();


}




void UVergjorn_Instance::RegisterGameMode(AVergjornGameModeBase* gm)
{

}


void UVergjorn_Instance::NewGame()
{
	auto world = GetWorld();
	UGameplayStatics::OpenLevel(world, FName("M_Base"));
}

void UVergjorn_Instance::Continue()
{
	auto world = GetWorld();
	UGameplayStatics::OpenLevel(world, FName("M_Base"));
}

void UVergjorn_Instance::LoadGame(FVergjornSaveGame save)
{
	auto world = GetWorld();


	UGameplayStatics::OpenLevel(world, FName("M_Base"));
}




void UVergjorn_Instance::ExitVergjorn()
{
	//FGenericPlatformMisc::RequestExit(false);
}
