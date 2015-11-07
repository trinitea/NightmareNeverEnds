// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "BasePlayerController.h"
#include "BaseCharacter.h"
#include "DefaultPlayerCameraManager.h"
#include "CameraSpringArmComponent.h"

/*
 *	Default constructor.
 */
ABasePlayerController::ABasePlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
	ThirdPersonCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("ThirdPersonCamera"));
}

/*
 *	Overriden input setup method. Sets our defaults input for the controlled
 *	character.
 */
void ABasePlayerController::SetupInputComponent()
{
	InputComponent = ConstructObject<UInputComponent>(UInputComponent::StaticClass(), this, TEXT("PC_InputComponent0"));
	InputComponent->RegisterComponent();

	//Check if valid
	check(InputComponent);

	//GAME PAD MAPPINGS
	TArray<FCustomBindingData> bindings1;
	bindings1.Add(FCustomBindingData(InputComponent->BindAxis("leftstick_vertical", this, &ABasePlayerController::MoveVertically)));
	bindings1.Add(FCustomBindingData(InputComponent->BindAxis("leftstick_horizontal", this, &ABasePlayerController::MoveHorizontally)));
	InputBindings.Add(GetEnumValue(EBindingAction::Movement), UCustomActionBindingComponent(bindings1));
		
	TArray<FCustomBindingData> bindings2;
	bindings2.Add(FCustomBindingData(InputComponent->BindAxis("rightstick_vertical", this, &ABasePlayerController::RotateCameraVertically)));
	bindings2.Add(FCustomBindingData(InputComponent->BindAxis("rightstick_horizontal", this, &ABasePlayerController::RotateCameraHorizontally)));
	InputBindings.Add(GetEnumValue(EBindingAction::CameraMovement), UCustomActionBindingComponent(bindings2));
	
	TArray<FCustomBindingData> bindings3;
	bindings3.Add(FCustomBindingData(InputComponent->BindAction("a_button", IE_Pressed, this, &ABasePlayerController::Jump)));
	bindings3.Add(FCustomBindingData(InputComponent->BindAction("a_button", IE_Released, this, &ABasePlayerController::StopJumping)));
	InputBindings.Add(GetEnumValue(EBindingAction::Jump), UCustomActionBindingComponent(bindings3));
		
	//KEYBOARD MAPPINGS
	InputComponent->BindAxis("mouse_y", this, &ABasePlayerController::AddPitchInput);
	InputComponent->BindAxis("mouse_x", this, &ABasePlayerController::AddYawInput);
}

/*
 *	Method that blocks the input for the Local player.
 */
void ABasePlayerController::BlockInput_Implementation(EBindingAction Action)
{
	UCustomActionBindingComponent* BindingComp = InputBindings.Find(GetEnumValue(Action));

	if (!BindingComp) return;

	// Unbind and lock it so it cannot be used
	BindingComp->UnbindAction(InputComponent);
	BindingComp->SetLocked(true);
}

/*
 *	Method that allows input for an action.
 */
void ABasePlayerController::AllowInput_Implementation(EBindingAction Action)
{
	UCustomActionBindingComponent* BindingComp = InputBindings.Find(GetEnumValue(Action));

	if (!BindingComp) return;

	// Unlock and bind action if binded by default
	BindingComp->SetLocked(false);

	// If is binded by default, bind the control, else wait for it to be binded
	if (BindingComp->IsBindedByDefault())
	{
		BindingComp->BindAction(InputComponent);
	}
}


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
	check(ThirdPersonCamera);
	
	CameraBoom->TargetArmLength = 300.0f;				//Boom distance (distance of the camera from the player
	CameraBoom->bUsePawnControlRotation = true;			//Use player controller rotation
	CameraBoom->AttachTo(GetRootComponent());			//Attach to root of player controller
	
	ThirdPersonCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);	//Attach camera to camera boom
	ThirdPersonCamera->bUsePawnControlRotation = false;							//Do not use player controller rotation
}

int32 ABasePlayerController::GetCharacterHealth(){
	int32 health = 0;

	ABaseCharacter* TheCharacter = dynamic_cast<ABaseCharacter*>(GetPawn());

	if (TheCharacter) {
		health = TheCharacter->Health;
	}

	return health;
}

/*
 *	Override of tick function to make the player controller follow 
 *	the character since bAttachToPawn does not work in multiplayer.
 */
void ABasePlayerController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (!GetPawn()) return;
	
	RootComponent->SetWorldLocation(GetPawn()->GetActorLocation());
}

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

/*
*	Method that initiates the jump sequence.
*/
void ABasePlayerController::Jump()
{
	//Do nothing if no pawn or not character
	if (GetPawn() == NULL || !dynamic_cast<ACharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	((ACharacter*)GetPawn())->Jump();
}

/*
*	Method that stops the jump sequence.
*/
void ABasePlayerController::StopJumping()
{
	//Do nothing if no pawn or not character
	if (GetPawn() == NULL || !dynamic_cast<ACharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	((ACharacter*)GetPawn())->StopJumping();
}