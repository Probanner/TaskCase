// Copyright Epic Games, Inc. All Rights Reserved.

#include "TaskCaseGameMode.h"
#include "TaskCaseHUD.h"
#include "TaskCaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATaskCaseGameMode::ATaskCaseGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
//	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATaskCaseHUD::StaticClass();
}
