// Copyright Epic Games, Inc. All Rights Reserved.

#include "K_YangGaengGameMode.h"
#include "K_YangGaengCharacter.h"
#include "UObject/ConstructorHelpers.h"

AK_YangGaengGameMode::AK_YangGaengGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
