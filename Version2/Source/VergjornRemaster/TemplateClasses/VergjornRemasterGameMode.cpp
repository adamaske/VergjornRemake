// Copyright Epic Games, Inc. All Rights Reserved.

#include "VergjornRemasterGameMode.h"
#include "VergjornRemasterPlayerController.h"
#include "VergjornRemasterCharacter.h"
#include "UObject/ConstructorHelpers.h"
AVergjornRemasterGameMode::AVergjornRemasterGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AVergjornRemasterPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}