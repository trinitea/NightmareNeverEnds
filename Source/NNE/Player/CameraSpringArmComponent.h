// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "CameraSpringArmComponent.generated.h"

/**
* Default Spring Arm for the third person camera boom.
*/
UCLASS()
class UCameraSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

		//METHODS
public:
	UCameraSpringArmComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime);

};
