// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "DefaultPlayerCameraManager.h"
#include "BasePlayerController.h"

/*
 *	Default constructor.
 */
ADefaultPlayerCameraManager::ADefaultPlayerCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

/*
 *	Overriden method that updates the view target. If the camera is controlled by the player controller
 *	rather than the pawn, we use that camera and the controller's camera position.
 */
void ADefaultPlayerCameraManager::UpdateViewTarget(FTViewTarget & OutVT, float DeltaTime)
{
	//START ORIGINAL UPDATEVIEW TARGET CODE

	// Don't update outgoing viewtarget during an interpolation 
	if ((PendingViewTarget.Target != NULL) && BlendParams.bLockOutgoing && OutVT.Equal(ViewTarget))
	{
		return;
	}
	
	//@TODO: CAMERA: Should probably reset the view target POV fully here
	OutVT.POV.FOV = DefaultFOV;
	OutVT.POV.OrthoWidth = DefaultOrthoWidth;
	OutVT.POV.bConstrainAspectRatio = false;
	OutVT.POV.ProjectionMode = bIsOrthographic ? ECameraProjectionMode::Orthographic : ECameraProjectionMode::Perspective;
	OutVT.POV.PostProcessBlendWeight = 1.0f;
	
	//END ORIGINAL UPDATEVIEW TARGET CODE
	
	//If type is a base player controller and has a camera
	if (dynamic_cast<ABasePlayerController*>(GetOwningPlayerController()) && ((ABasePlayerController*)GetOwningPlayerController())->GetCamera() != NULL)
	{
		ABasePlayerController* playerController = (ABasePlayerController*) GetOwningPlayerController();
		if (playerController->GetPawn())
		{
			//Rotate camera boom according to view rotation
			const FRotator pawnViewRotation = playerController->GetPawn()->GetViewRotation();
			if (!pawnViewRotation.Equals(playerController->GetCameraBoom()->GetComponentRotation()))
			{
				playerController->GetCameraBoom()->SetWorldRotation(pawnViewRotation);
			}
		}

		UCameraComponent* camera = playerController->GetCamera();

		//Set POV variables
		OutVT.POV.Location = camera->GetComponentLocation();
		OutVT.POV.Rotation = camera->GetComponentRotation();
		OutVT.POV.FOV = camera->FieldOfView;
		OutVT.POV.AspectRatio = camera->AspectRatio;
		OutVT.POV.bConstrainAspectRatio = camera->bConstrainAspectRatio;
		OutVT.POV.ProjectionMode = camera->ProjectionMode;
		OutVT.POV.OrthoWidth = camera->OrthoWidth;
		OutVT.POV.PostProcessBlendWeight = camera->PostProcessBlendWeight;
		if (camera->PostProcessBlendWeight > 0.0f)
		{
			OutVT.POV.PostProcessSettings = camera->PostProcessSettings;
		}

		//Apply camera stuff
		ApplyCameraModifiers(DeltaTime, OutVT.POV);
		SetActorLocationAndRotation(OutVT.POV.Location, OutVT.POV.Rotation, false);
		UpdateCameraLensEffects(OutVT);
	}
	//If not base player controller or no camera, then call default method 
	else
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
	}
 }
