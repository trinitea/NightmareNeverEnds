// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePlayerController.h"
#include "KidPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AKidPlayerController : public ABasePlayerController
{
	GENERATED_BODY()

public:
	//Constructor
	AKidPlayerController(const FObjectInitializer& ObjectInitializer);
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "Controller", meta = (UIMin = "0.0", UIMax = "1.0"))
	float TriggerPressureThreshold = 0.1f;

private:
	bool bGlideTrigger = false;

	void GlideManager(float value);
	void Slash();

};
