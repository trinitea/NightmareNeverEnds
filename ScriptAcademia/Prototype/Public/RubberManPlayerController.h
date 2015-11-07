// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePlayerController.h"
#include "RubberManPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API ARubberManPlayerController : public ABasePlayerController
{
	GENERATED_BODY()
		
public:
	ARubberManPlayerController(const FObjectInitializer& ObjectInitializer);
	virtual void SetupInputComponent() override;

protected:
	void HoldHands();
	void HoldFeet();
	void ReleaseHands();
	void ReleaseFeet();
	void VStretch(float Value);
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
};
