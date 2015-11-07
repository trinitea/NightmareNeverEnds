// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "NNE.h"
#include "NNEGameMode.h"
#include "NNEPawn.h"

ANNEGameMode::ANNEGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ANNEPawn::StaticClass();
}

