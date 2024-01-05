// Fill out your copyright notice in the Description page of Project Settings.


#include "V_Controller.h"
#include "V_Player.h"

#include "UI/V_HUD.h"

#include "SaveAndLoadSubsystem.h"
//Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
void AV_Controller::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	mPlayer = Cast<AV_Player>(pawn);
	if (!mPlayer) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("PC : Found no Player")));
	}

}

void AV_Controller::BeginPlay() {
	Super::BeginPlay();
	//Set mouse
	//SetShowMouseCursor(true);
	//Create HUD

	/* Mapping Context */
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (m_MenusMappingContext) {
			Subsystem->AddMappingContext(m_MenusMappingContext, 0);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("PC : No Menu Mapping Context")));
		}

	}

	auto GI = GetGameInstance();
	auto ss = GI->GetSubsystem<USaveAndLoadSubsystem>();
	if (ss) {
		ss->LoadAndCacheSaves();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("PC : No SS")));
	}
	//if (AV_HUD* hud = CastChecked<AV_HUD>(GetHUD())){
	//	m_HUD = hud;
	//
	//}
	//else {
	//	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("PC : No HUD found!")));
	//}
}
void AV_Controller::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);
}

void AV_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(m_PauseAction, ETriggerEvent::Started, this, &AV_Controller::Pause);
		EnhancedInputComponent->BindAction(m_QuicksaveAction, ETriggerEvent::Started, this, &AV_Controller::Quicksave);
		EnhancedInputComponent->BindAction(m_QuickloadAction, ETriggerEvent::Started, this, &AV_Controller::Quickload);

	}
}

void AV_Controller::Pause()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("PC : Pause")));

}

void AV_Controller::SwitchInputMode(E_V_InputType type)
{
	m_InputType = type;

	m_InputTypeSwitched.Broadcast(m_InputType);
}

void AV_Controller::Quicksave()
{
	auto GI = GetGameInstance();
	auto ss = GI->GetSubsystem<USaveAndLoadSubsystem>();
	if (ss) {
		ss->Quicksave();
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("PC : No SS")));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("PC : No SS")));
	}
}

void AV_Controller::Quickload()
{
	auto GI = GetGameInstance();
	auto ss = GI->GetSubsystem<USaveAndLoadSubsystem>();
	if (ss) {
		ss->Quickload();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("PC : No SS")));
	}
}
