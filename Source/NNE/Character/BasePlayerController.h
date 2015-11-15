#pragma once

#include "GameFramework/Character.h"
#include "BasePlayerController.generated.h"

UCLASS(Blueprintable)
class ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();

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

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName ActionForwardBinding;
	static const FName ActionRightBinding;
	static const FName FireMainBinding;
	static const FName FireSecondaryBinding;

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

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	//void SetupPlayerInputComponent(class UInputComponent* InputComponent);

	void Tick(float Deltaseconds);
	void ControlPawn(float DeltaSecond);
	//void RotateCameraVertically(float rate);
	//void RotateCameraHorizontally(float rate);
	
};
