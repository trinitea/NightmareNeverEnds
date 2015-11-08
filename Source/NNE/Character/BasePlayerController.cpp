#include "NNE.h"
#include "BasePlayerController.h"

#include "BaseCharacter.h"
#include "Player/DefaultPlayerCameraManager.h"
#include "Player/CameraSpringArmComponent.h"

ABasePlayerController::ABasePlayerController(const FObjectInitializer& ObjectInitializer)
{
	//Define camera rotation rates
	CameraVerticalRotationRate = 45.f;
	CameraHorizontalRotationRate = 45.f;

	//Attach player controller to pawn
	bAttachToPawn = true;

	//Define custom camera manager IMPORTANT, else third person camera won't work
	PlayerCameraManagerClass = ADefaultPlayerCameraManager::StaticClass();

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<UCameraSpringArmComponent>(this, TEXT("CameraBoom"));

	// Create a follow camera
	//CameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("ThirdPersonCamera"));
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm
};

void ABasePlayerController::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);

	// set up gameplay key bindings
	InputComponent->BindAxis(MoveForwardBinding);
	InputComponent->BindAxis(MoveRightBinding);
	InputComponent->BindAxis(UseForwardBinding);
	InputComponent->BindAxis(UseRightBinding);
}

//==============================================================================
// BELOW IS FOR TESTING
//==============================================================================

/*
*	Method that moves the player from front to back or vice versa.
*/
void ABasePlayerController::MoveVertically(float value)
{
	if (value == 0.0f) return;

	//Do nothing if no pawn
	if (GetPawn() == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	const FRotator YawRotation(0, ControlRotation.Yaw, 0);

	// get direction
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	//Move pawn
	GetPawn()->AddMovementInput(Direction, value);
}

/*
*	Method that moves the player from left to right or vice versa.
*/
void ABasePlayerController::MoveHorizontally(float value)
{
	if (value == 0.0f) return;

	//Do nothing if no pawn
	if (GetPawn() == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	const FRotator YawRotation(0, ControlRotation.Yaw, 0);

	// get move direction
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//Move pawn
	GetPawn()->AddMovementInput(Direction, value);
}

/*
*	Method that moves the camera vertically.
*/
void ABasePlayerController::RotateCameraVertically(float rate)
{
	//Reverse if not inverted axis
	rate = (bInvertAxisY) ? rate : rate * -1;

	//Rotate camera
	AddPitchInput(rate * CameraVerticalRotationRate * GetWorld()->GetDeltaSeconds());
}

/*
*	Method that moves the camera horizontally.
*/
void ABasePlayerController::RotateCameraHorizontally(float rate)
{
	AddYawInput(rate * CameraHorizontalRotationRate * GetWorld()->GetDeltaSeconds());
}