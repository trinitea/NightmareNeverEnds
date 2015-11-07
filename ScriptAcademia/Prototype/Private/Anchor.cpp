// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "Anchor.h"

void UAnchor::Build()
{
	// Initialize the constraint
	joint = NewObject<UPhysicsConstraintComponent>(this);
	Constrain(false);
}

void UAnchor::Constrain(bool constrain)
{
	// Fixed joint logic
	isGrabbing = constrain;			// This boolean keeps track of the anchor state
	ELinearConstraintMotion lcm;	// Linear constraint motion placeholder
	EAngularConstraintMotion acm;	// Angular constraint motion placeholder
	if (constrain)
	{
		// Lock fixed joint
		lcm = ELinearConstraintMotion::LCM_Locked;
		acm = EAngularConstraintMotion::ACM_Locked;
	}
	else
	{
		// Free fixed join
		lcm = ELinearConstraintMotion::LCM_Free;
		acm = EAngularConstraintMotion::ACM_Free;
	}
	// Set constraint motion
	joint->SetLinearXLimit(lcm, 0);
	joint->SetLinearYLimit(lcm, 0);
	joint->SetLinearZLimit(lcm, 0);
	joint->SetAngularSwing1Limit(acm, 0);
	joint->SetAngularSwing2Limit(acm, 0);
	joint->SetAngularTwistLimit(acm, 0);
	// Disable collisions between linked objects (useless for now)
	joint->SetDisableCollision(true);
	// Refresh the constraint
	joint->SetConstrainedComponents(this, NAME_None, NULL, NAME_None);
}

void UAnchor::AnchorTick(float DeltaTime)
{
	if (joint != nullptr && keyPressed && !isGrabbing && CanHold())
	{
		UE_LOG(LogTemp, Log, TEXT("Grabbing"));
		joint->SetWorldLocation(this->GetComponentLocation());
		Constrain(true);
	}
	else if (joint != nullptr && !keyPressed && isGrabbing)
	{
		UE_LOG(LogTemp, Log, TEXT("Releasing"));
		joint->UpdateConstraintFrames();
		Constrain(false);
		joint->UpdateConstraintFrames();
	}
}

// Returns true if the anchor can hold an object
bool UAnchor::CanHold()
{
	// Test overlapSphere
	TArray<FOverlapResult> overlaps;
	FVector pos = this->ComponentToWorld.GetLocation();
	float rad = this->SphereRadius * grabRadiusMultiplier;
	auto collisionParams = FCollisionQueryParams(NAME_None, false);
	GetWorld()->OverlapMulti(overlaps, pos, FQuat::Identity, FCollisionShape::MakeSphere(rad), collisionParams, FCollisionObjectQueryParams::AllStaticObjects);
	bool boxOverlapTestRes = false;
	int colCount = 0;
	for (int idx = 0; idx < overlaps.Num(); ++idx)
	{
		colCount++;
	}
	return (colCount > 0);
}