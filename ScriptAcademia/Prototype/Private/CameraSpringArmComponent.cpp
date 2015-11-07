// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "CameraSpringArmComponent.h"

/*
 *	Default constructor.
 */
UCameraSpringArmComponent::UCameraSpringArmComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}


/*
 *	Overriden Method to update the arm location. Overriden so that we can ignore
 *	the collision between the arm and the pawn.
 */
void UCameraSpringArmComponent::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime)
{
	//START CODE FROM SUPER METHOD

	FRotator DesiredRot = GetComponentRotation();

	// If inheriting rotation, check options for which components to inherit
	if (!bAbsoluteRotation)
	{
		if (!bInheritPitch)
		{
			DesiredRot.Pitch = RelativeRotation.Pitch;
		}

		if (!bInheritYaw)
		{
			DesiredRot.Yaw = RelativeRotation.Yaw;
		}

		if (!bInheritRoll)
		{
			DesiredRot.Roll = RelativeRotation.Roll;
		}
	}

	// Apply 'lag' to rotation if desired
	if (bDoRotationLag)
	{
		DesiredRot = FMath::RInterpTo(PreviousDesiredRot, DesiredRot, DeltaTime, CameraRotationLagSpeed);
	}
	PreviousDesiredRot = DesiredRot;

	// Get the spring arm 'origin', the target we want to look at
	FVector ArmOrigin = GetComponentLocation() + TargetOffset;
	// We lag the target, not the actuall camera position, so rotating the camera around does not hav lag
	FVector DesiredLoc = ArmOrigin;
	if (bDoLocationLag)
	{
		DesiredLoc = FMath::VInterpTo(PreviousDesiredLoc, DesiredLoc, DeltaTime, CameraLagSpeed);
	}
	PreviousDesiredLoc = DesiredLoc;

	// Now offset camera position back along our rotation
	DesiredLoc -= DesiredRot.Vector() * TargetArmLength;
	// Add socket offset in local space
	DesiredLoc += FRotationMatrix(DesiredRot).TransformVector(SocketOffset);

	// Do a sweep to ensure we are not penetrating the world
	FVector ResultLoc;
	if (bDoTrace && (TargetArmLength != 0.0f))
	{
		static FName TraceTagName(TEXT("SpringArm"));

	//END  CODE FROM SUPER METHOD

		//If there is an attached pawn, ignore its collision
		AActor* ignoreActor = NULL;
		if (dynamic_cast<APlayerController*>(GetOwner()))
		{
			ignoreActor = ((APlayerController*)GetOwner())->GetPawn();
		}
		
		if (!ignoreActor)
		{
			ignoreActor = GetOwner();
		}

		FCollisionQueryParams QueryParams(TraceTagName, false, ignoreActor);	

	//START CODE FROM SUPER METHOD

		FHitResult Result;
		GetWorld()->SweepSingle(Result, ArmOrigin, DesiredLoc, FQuat::Identity, ProbeChannel, FCollisionShape::MakeSphere(ProbeSize), QueryParams);

		ResultLoc = BlendLocations(DesiredLoc, Result.Location, Result.bBlockingHit, DeltaTime);
	}
	else
	{
		ResultLoc = DesiredLoc;
	}

	// Form a transform for new world transform for camera
	FTransform WorldCamTM(DesiredRot, ResultLoc);
	// Convert to relative to component
	FTransform RelCamTM = WorldCamTM.GetRelativeTransform(ComponentToWorld);

	// Update socket location/rotation
	RelativeSocketLocation = RelCamTM.GetLocation();
	RelativeSocketRotation = RelCamTM.GetRotation();

	UpdateChildTransforms();

	//END CODE FROM SUPER METHOD
}