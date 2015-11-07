// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "Anchor.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class UAnchor : public USphereComponent
{
	GENERATED_BODY()

public:
	bool keyPressed = false;
	bool isGrabbing = false;
	float grabRadiusMultiplier = 1.40f;
	void Build();
	void Constrain(bool constrain);
	void AnchorTick(float DeltaTime);
	bool CanHold();
	UPhysicsConstraintComponent* joint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		FString anchorName;

private:
	// TODO: FixedJoint* fixedJoint; <-- DOES NOT EXIST IN UE4
	// TODO: Ignore collision with itself, rubberman and the trampoline
};