// Copyright Epic Games, Inc. All Rights Reserved.

#include "PhoneControllerGameMode.h"
#include "PhoneControllerHUD.h"
#include "PhoneControllerCharacter.h"
#include "UObject/ConstructorHelpers.h"

APhoneControllerGameMode::APhoneControllerGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APhoneControllerHUD::StaticClass();
}
