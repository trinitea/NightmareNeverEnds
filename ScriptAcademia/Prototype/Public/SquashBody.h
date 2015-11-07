// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "SquashBody.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API USquashBody : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	float tension = 0.0f; // Should be set by Elastic
	float tensionThreshold = 150.0f; // Tension that is equivalent to the maximum bounciness
	float maxBounciness = 1.0f;
	float minBounciness = 0.2f;
	
	void Update();
};
