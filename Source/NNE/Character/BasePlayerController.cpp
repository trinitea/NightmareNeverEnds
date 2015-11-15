#include "NNE.h"
#include "BasePlayerController.h"

#include "BaseCharacter.h"
#include "Player/DefaultPlayerCameraManager.h"
#include "Player/CustomActionBindingComponent.h"
#include "Player/CameraSpringArmComponent.h"

const FName ABasePlayerController::MoveForwardBinding("MoveForward");
const FName ABasePlayerController::MoveRightBinding("MoveRight");
const FName ABasePlayerController::ActionForwardBinding("ActionForward");
const FName ABasePlayerController::ActionRightBinding("ActionRight");
const FName ABasePlayerController::FireMainBinding("FireMain");
const FName ABasePlayerController::FireSecondaryBinding("FireSecondary");

ABasePlayerController::ABasePlayerController()
{
	//Define camera rotation rates
	//CameraVerticalRotationRate = 45.f;
	//CameraHorizontalRotationRate = 45.f;

	//Attach player controller to pawn
	bAttachToPawn = true;

	//Define custom camera manager IMPORTANT, else third person camera won't work
	PlayerCameraManagerClass = ADefaultPlayerCameraManager::StaticClass();

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<UCameraSpringArmComponent>(TEXT("CameraBoom"));

	// Create a follow camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("IsometricCamera"));

	
};

/*
*	Overriden Begin play method. Does the setup of the camera boom and third person
*	camera.
*/
void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// SUPER IMPORTANT!!! Else pawn is not possessed by default (which is quite stupid btw Unreal)
	Possess(GetPawn());

	//Check if not null, since blueprints are somethime corrupted
	check(CameraBoom);
	check(CameraComponent);

	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 900.f;
	//CameraBoom->RelativeRotation = FRotator(-65.f, 0.f, 0.f);
	AddPitchInput(40.f); // for unknown reason, the boom inherite the controller rotation
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraBoom->AttachTo(RootComponent);

	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm
	CameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
}

void ABasePlayerController::SetupInputComponent()
{
	InputComponent = NewObject<UInputComponent>(this, TEXT("PC_InputComponent0"));
	InputComponent->RegisterComponent();

	UE_LOG(LogTemp, Error, TEXT("Been there, done that"));
	check(InputComponent);

	// XYdouble axis 1
	InputComponent->BindAxis(MoveForwardBinding);
	InputComponent->BindAxis(MoveRightBinding);

	// XYdouble axis 2
	InputComponent->BindAxis(ActionForwardBinding);
	InputComponent->BindAxis(ActionRightBinding);

	// Z axis 1
	InputComponent->BindAxis(FireMainBinding);
	// Z axis 2
	InputComponent->BindAxis(FireSecondaryBinding);

	// XY input
}

void ABasePlayerController::Tick(float DeltaSeconds){

	Super::Tick(DeltaSeconds);

	ControlPawn(DeltaSeconds);
}

void ABasePlayerController::ControlPawn(float DeltaSeconds){
	
	if (GetPawn() == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	ABaseCharacter* character = Cast<ABaseCharacter>(GetPawn());
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);

	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);
	character->Move(ForwardValue, RightValue, DeltaSeconds, YawRotation);

	const float ActionForwardValue = GetInputAxisValue(ActionForwardBinding);
	const float ActionRightValue = GetInputAxisValue(ActionRightBinding);
	character->Aim(ActionForwardValue, ActionRightValue, YawRotation);
	
	// fire 1 and 2
	const float Fire1Value = GetInputAxisValue(FireMainBinding);
	const float Fire2Value = GetInputAxisValue(ActionForwardBinding);
	if (Fire1Value > 0.2f) character->UseMain();
	
	
}

//==============================================================================
// BELOW IS FOR TESTING
//==============================================================================

/*
void ABasePlayerController::MoveVertically(float value)
{
	if (value == 0.0f) return;

	//Do nothing if no pawn
	if (GetControlledPawn() == NULL)
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
}*/

/*
void ABasePlayerController::RotateCamera(float rate)
{
	//Reverse if not inverted axis
	rate = (bInvertAxisY) ? rate : rate * -1;

	//Rotate camera
	AddPitchInput(rate * CameraVerticalRotationRate * GetWorld()->GetDeltaSeconds());
}*/