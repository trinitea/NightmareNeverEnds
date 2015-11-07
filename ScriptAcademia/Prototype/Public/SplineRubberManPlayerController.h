// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePlayerController.h"
#include "SplineRubberManPlayerController.generated.h"

/**
*
*/
UCLASS()
class PROTOTYPE_API ASplineRubberManPlayerController : public ABasePlayerController
{
	GENERATED_BODY()


public:
	ASplineRubberManPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void SetupInputComponent() override;

private:
	void StartStretch();
	void ReleaseStretch();

};
