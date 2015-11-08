// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "DefaultPlayerCameraManager.generated.h"

/**
*	Class for the default camera manager. Necessary if we want to control the camera
*	from the player controller.
*/
UCLASS()
class ADefaultPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

		//METHODS
public:
	ADefaultPlayerCameraManager(const FObjectInitializer& ObjectInitializer);

	virtual void UpdateViewTarget(FTViewTarget & OutVT, float DeltaTime) override;

};
