// Fill out your copyright notice in the Description page of Project Settings.

#include "NNE.h"
#include "Character/BaseCharacter.h"
#include "MainGameMode.h"

AMainGameMode::AMainGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ABaseCharacter::StaticClass();
}


