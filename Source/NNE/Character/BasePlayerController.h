#pragma once

#include "GameFramework/Character.h"
#include "BasePlayerController.generated.h"

//const FName ABasePlayerController::MoveForwardBinding("MoveForward");
//const FName ABasePlayerController::MoveRightBinding("MoveRight");
//const FName ABasePlayerController::UseForwardBinding("FireForward");
//const FName ABasePlayerController::UseRightBinding("FireRight");

UCLASS(Blueprintable)
class ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController(const FObjectInitializer& ObjectInitializer);

//==============================================================================
// PROPERTIES
//==============================================================================

public:

	/** The camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//If Y axis is inverted
	bool bInvertAxisY = false;

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	float CameraVerticalRotationRate;

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	float CameraHorizontalRotationRate;

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName UseForwardBinding;
	static const FName UseRightBinding;

private:
	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

//==============================================================================
// FUNCTIONS
//==============================================================================
public:

	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	void MoveVertically(float value); // can be in base controller
	void MoveHorizontally(float value); // can be in base controller
	void RotateCameraVertically(float rate);
	void RotateCameraHorizontally(float rate);
	
	void SetupPlayerInputComponent(class UInputComponent* InputComponent);
};
