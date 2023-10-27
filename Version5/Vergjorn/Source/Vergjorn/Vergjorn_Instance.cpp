// Fill out your copyright notice in the Description page of Project Settings.


#include "Vergjorn_Instance.h"
#include "Kismet/GameplayStatics.h"

UVergjorn_Instance::UVergjorn_Instance()
{
}

void UVergjorn_Instance::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("GI Init")));
	
	//Create Vergjorn
	//mVergjorn = NewObject<UV_Vergjorn>();

	//Vergjorn
	//mVergjorn->Init();

	Load_Saves();
}

void UVergjorn_Instance::Shutdown()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("GI Shutdown")));


	//mVergjorn->Shutdown();


}

void UVergjorn_Instance::Load_Saves()
{
	//Creates fake files
	int N = 10;
	auto unsorted_array = TArray<FVergjorn_SaveGame>();
	for (int i = 0; i < N; i++)
	{
		auto g = FVergjorn_SaveGame();
		g.m_Title = "Save_" + FString::FromInt(i);

		unsorted_array.Add(g);
		FPlatformProcess::Sleep(0.05);
	}

	m_Loaded_GameSaves = Sort_Saves(unsorted_array);
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

void UVergjorn_Instance::LoadGame(FVergjorn_SaveGame save)
{
	auto world = GetWorld();
	UGameplayStatics::OpenLevel(world, FName("M_Base"));
}

TArray<FVergjorn_SaveGame> UVergjorn_Instance::Get_SaveGames()
{
	return m_Loaded_GameSaves;
}

void UVergjorn_Instance::ExitVergjorn()
{
	FGenericPlatformMisc::RequestExit(false);

}

TArray<FVergjorn_SaveGame> UVergjorn_Instance::Sort_Saves(TArray<FVergjorn_SaveGame> files)
{
	//Bubble sort algorithm
	auto unsorted_array = files;	
	int n = unsorted_array.Num();
	int i, j;
	bool swapped;
	for (i = 0; i < n - 1; i++) {
		swapped = false;
		for (j = 0; j < n - i - 1; j++) {
			if (unsorted_array[j].m_DateTime < unsorted_array[j + 1].m_DateTime) {
				unsorted_array.Swap(j, j + 1);
				swapped = true;
			}
		}
		if (swapped == false)
			break;
	}
	return unsorted_array;
}

