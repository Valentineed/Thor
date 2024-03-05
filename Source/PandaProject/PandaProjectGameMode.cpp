// Copyright Epic Games, Inc. All Rights Reserved.

#include "PandaProjectGameMode.h"
#include "PandaProjectPlayerController.h"
#include "PandaProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "LifeComponent.h"

APandaProjectGameMode::APandaProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = APandaProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerControllerClass == nullptr)
		return;
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

