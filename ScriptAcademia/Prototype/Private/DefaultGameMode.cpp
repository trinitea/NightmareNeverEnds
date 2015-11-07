// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "DefaultGameMode.h"
#include "RubberManPlayerController.h"
#include "RubberManCharacter.h"

/*
 *	Default constructor.
 */
ADefaultGameMode::ADefaultGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//Player Controller is blueprinted player controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("Blueprint'/Game/Blueprints/RubberManPlayerControllerBP'"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/Blueprints/RubberManCharacterBP'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

