// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "DefaultGameMode.generated.h"

/**
 *	Class for the game mode 
 */
UCLASS()
class PROTOTYPE_API ADefaultGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADefaultGameMode(const FObjectInitializer& ObjectInitializer);
	
};
