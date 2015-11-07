// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "RubberManCharacter.h"
#include "CameraSpringArmComponent.h"
#include "BasePlayerController.h"

ARubberManCharacter::ARubberManCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

// Component registration
void ARubberManCharacter::BeginPlay()
{
	// Get all the required Anchors in this actor
	UE_LOG(LogTemp, Log, TEXT("Finding Anchors..."));
	TArray<UAnchor*> anchors;
	GetComponents(anchors);
	for (UAnchor* anchor : anchors)
	{
		UE_LOG(LogTemp, Log, TEXT("Anchor found: %s"), *anchor->anchorName);
		if (anchor->anchorName.Equals("Hands")) this->hands = anchor;
		if (anchor->anchorName.Equals("Feet"))  this->feet = anchor;
	}
	if (hands != NULL && feet != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Anchors found."));
		hands->Build();
		feet->Build();
		handsInitPosition = hands->RelativeLocation;
		feetInitPosition = feet->RelativeLocation;
		naturalLength = FVector::Dist(handsInitPosition, feetInitPosition);
		UE_LOG(LogTemp, Log, TEXT("Natural length: %f"), naturalLength);
		hands->SetSimulatePhysics(false);
		feet->SetSimulatePhysics(false);
		hands->SetHiddenInGame(true);
		feet->SetHiddenInGame(true);
		hands->IgnoreActorWhenMoving(this, true);
		feet->IgnoreActorWhenMoving(this, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Anchors not found."));
	}

	// Find the body
	UE_LOG(LogTemp, Log, TEXT("Finding body..."));
	TArray<USquashBody*> bodies;
	GetComponents(bodies);
	for (USquashBody* body : bodies)
	{
		this->body = body;
	}
	if (hands != NULL && feet != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Body found."));
		body->SetSimulatePhysics(false);
		body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		body->IgnoreActorWhenMoving(this, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Body not found."));
	}

	cam = NULL;

	// Find the cam
	if ((ABasePlayerController*)GetController() != NULL)
		cam = ((ABasePlayerController*)GetController())->GetCameraBoom();
	else
		UE_LOG(LogTemp, Warning, TEXT("Cam not found."));
}

void ARubberManCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARubberManCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARubberManCharacter::MoveForward(float Value)
{
	ServerMoveForward(Value);
}

void ARubberManCharacter::ServerMoveForward_Implementation(float Value)
{
	MulticastMoveForward(Value);
}

bool ARubberManCharacter::ServerMoveForward_Validate(float Value)
{
	return true;
}

void ARubberManCharacter::MulticastMoveForward_Implementation(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (mode == ControlMode::Default)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
		else if (mode == ControlMode::Elastic)
		{
			// Find out which way is forward
			FRotator camRot(0, Controller->GetControlRotation().Yaw, 0);
			FVector directionVector = FRotationMatrix(camRot).GetUnitAxis(EAxis::X);
			// Normalize the vector
			if (directionVector.Normalize())
			{
				// Control the anchor
				forwardV = directionVector;
				forwardValue = Value;
			}
			else forwardValue = 0;
		}
	}
	else forwardValue = 0;
}

void ARubberManCharacter::MoveRight(float Value)
{
	ServerMoveRight(Value);
}

void ARubberManCharacter::ServerMoveRight_Implementation(float Value)
{
	MulticastMoveRight(Value);
}

bool ARubberManCharacter::ServerMoveRight_Validate(float Value)
{
	return true;
}

void ARubberManCharacter::MulticastMoveRight_Implementation(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (mode == ControlMode::Default)
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
		else if (mode == ControlMode::Elastic)
		{
			// Find out which way is forward
			FRotator camRot(0, Controller->GetControlRotation().Yaw, 0);
			FVector directionVector = FRotationMatrix(camRot).GetUnitAxis(EAxis::Y);
			// Normalize the vector
			if (directionVector.Normalize())
			{
				rightV = directionVector;
				rightValue = Value;
				// Control the anchor
				//Stretch(directionVector, controlledAnchor, grabbingAnchor, Value);
			}
			else forwardValue = 0;
		}
	}
	else rightValue = 0;
}


void ARubberManCharacter::Stretch(UAnchor* controlledAnchor, UAnchor* grabbingAnchor)
{
	ServerStretch(controlledAnchor, grabbingAnchor);
}

void ARubberManCharacter::ServerStretch_Implementation(UAnchor* controlledAnchor, UAnchor* grabbingAnchor)
{
	MulticastStretch(controlledAnchor, grabbingAnchor);
}

bool ARubberManCharacter::ServerStretch_Validate(UAnchor* controlledAnchor, UAnchor* grabbingAnchor)
{
	return true;
}

void ARubberManCharacter::MulticastStretch_Implementation(UAnchor* controlledAnchor, UAnchor* grabbingAnchor)
{
	if (forwardValue == 0 && rightValue == 0 && upValue == 0) return;

	// Stretch the anchor
	FVector direction = (upValue * upV) + (rightValue * rightV) + (forwardValue * forwardV);
	FVector controlledAnchorPos = controlledAnchor->GetComponentLocation();
	FVector grabbingAnchorPos = grabbingAnchor->GetComponentLocation();
	float alphaVelocity = maxDist / FVector::Dist(controlledAnchorPos, grabbingAnchorPos);
	FVector force = alphaVelocity * direction * strength;

	// Equalize the character vertically if no vertical force is applied
	float adjustZ = 0.0f;
	if (upValue == 0 && (rightValue != 0 || forwardValue != 0))
	{
		float zDiff = FMath::Abs(controlledAnchorPos.Z - grabbingAnchorPos.Z);
		if (controlledAnchorPos.Z < grabbingAnchorPos.Z) adjustZ = 5.0f * zDiff;
		if (controlledAnchorPos.Z > grabbingAnchorPos.Z) adjustZ = -5.0f * zDiff;
	}

	// Equalize the character horizontally if no vertical force is applied
	float adjustX = 0.0f;
	float adjustY = 0.0f;
	if (rightValue == 0 && forwardValue == 0 && upValue != 0)
	{
		float xDiff = FMath::Abs(controlledAnchorPos.X - grabbingAnchorPos.X);
		if (controlledAnchorPos.X < grabbingAnchorPos.X) adjustX = 5.0f * xDiff;
		if (controlledAnchorPos.X > grabbingAnchorPos.X) adjustX = -5.0f * xDiff;
		float yDiff = FMath::Abs(controlledAnchorPos.Y - grabbingAnchorPos.Y);
		if (controlledAnchorPos.Y < grabbingAnchorPos.Y) adjustY = 5.0f * yDiff;
		if (controlledAnchorPos.Y > grabbingAnchorPos.Y) adjustY = -5.0f * yDiff;
	}

	controlledAnchor->SetAllPhysicsLinearVelocity(FVector(force.X + adjustX, force.Y + adjustY, force.Z + adjustZ), false);
}

void ARubberManCharacter::VStretch(float Value)
{
	ServerVStretch(Value);
}

void ARubberManCharacter::ServerVStretch_Implementation(float Value)
{
	MulticastVStretch(Value);
}

bool ARubberManCharacter::ServerVStretch_Validate(float Value)
{
	return true;
}

void ARubberManCharacter::MulticastVStretch_Implementation(float Value)
{
	upV = FVector::UpVector;
	upValue = Value;
}

void ARubberManCharacter::OnConstruction(const FTransform & transform)
{
}

void ARubberManCharacter::Tick(float DeltaTime)
{
	// The game won't work if these variables aren't initialized. Therefore, this code it useless.
	// However, the editor will run the Tick() function and will crash if the null checks are not made.
	if (hands == NULL || feet == NULL || body == NULL || cam == NULL) { UE_LOG(LogTemp, Log, TEXT("Unable to find components.")); BeginPlay(); return; }

	if (hands->keyPressed && hands->CanHold() && mode == ControlMode::Default) SetControlMode(ControlMode::Elastic);
	else if (feet->keyPressed  && feet->CanHold() && mode == ControlMode::Default) SetControlMode(ControlMode::Elastic);
	hands->AnchorTick(DeltaTime); // For some reasons, the TickComponent() function in the anchor component doesn't work.
	feet->AnchorTick(DeltaTime);  // So... let's call it here, for the sake of simplicity D:

	FVector handsPos = hands->GetComponentToWorld().GetLocation();
	FVector feetPos = feet->GetComponentToWorld().GetLocation();
	FVector midPoint = FMath::Lerp(handsPos, feetPos, 0.5f);

	// Default behavior
	if (mode == ControlMode::Default)
	{
		// Move the anchors
		UCapsuleComponent* cap = GetCapsuleComponent();
		cap = (UCapsuleComponent*)this->RootComponent;
		FVector capPos = cap->ComponentToWorld.GetLocation();

		// Adapt brake speed and friction accordingly to velocity (this prevents the sling shot)
		float frictionAlpha = 1 - (GetCharacterMovement()->Velocity.Size() / 1500.0f);
		GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f * frictionAlpha;
		GetCharacterMovement()->GroundFriction = 5.5f * frictionAlpha + 2.5f;
	}
	// Elastic behavior
	else if (mode == ControlMode::Elastic)
	{
		// Execute elastic physics
		ElasticPhysics(handsPos, feetPos);

		// Stretch
		UAnchor* controlledAnchor = NULL;
		UAnchor* grabbingAnchor = NULL;
		if (hands->isGrabbing && !feet->isGrabbing)			{ controlledAnchor = feet;  grabbingAnchor = hands; }
		else if (!hands->isGrabbing && feet->isGrabbing)	{ controlledAnchor = hands; grabbingAnchor = feet; }
		if (controlledAnchor != NULL && grabbingAnchor != NULL)
		{
			Stretch(controlledAnchor, grabbingAnchor);
		}

		// Position the camera
		cam->TargetArmLength = 300.0f + FVector::Dist(handsPos, feetPos);

		// Restitution
		if (FVector::Dist(handsPos, feetPos) < naturalLength && !hands->isGrabbing && !feet->isGrabbing)
		{
			UE_LOG(LogTemp, Log, TEXT("RESTITUTION"));
			SetControlMode(ControlMode::Default);
		}
	}
}

void ARubberManCharacter::ElasticPhysics(FVector handsPos, FVector feetPos)
{
	FVector midPoint = FMath::Lerp(handsPos, feetPos, 0.5f);
	CalculateTension(handsPos, feetPos);

	// Calculate elastic forces
	FVector forceHands = tension * (midPoint - handsPos).SafeNormal();
	FVector forceFeet = tension *  (midPoint - feetPos).SafeNormal();

	// Apply elastic forces to both anchors 
	hands->AddForce(forceHands);
	feet->AddForce(forceFeet);

	// Update body
	body->SetWorldLocation(midPoint);
	body->SetWorldRotation((handsPos - feetPos).Rotation());
	float bodyLength = FVector::Dist(handsPos, feetPos);
	body->SetWorldScale3D(FVector(bodyLength / 62.5f, 1.0f, 0.2f));

	// Update trampoline tension
	// trampoline->tension = tension;
}

void ARubberManCharacter::CalculateTension(FVector aPos, FVector bPos)
{
	float d = FVector::Dist(aPos, bPos);
	if (d - naturalLength > 0) tension = elasticity * FMath::Pow(d - naturalLength, 2);
	else tension = 0;
}

void ARubberManCharacter::HoldHands()
{
	ServerHoldHands();
}

void ARubberManCharacter::ServerHoldHands_Implementation()
{
	MulticastHoldHands();
}

bool ARubberManCharacter::ServerHoldHands_Validate()
{
	return true;
}

void ARubberManCharacter::MulticastHoldHands_Implementation()
{
	hands->keyPressed = true;
}

void ARubberManCharacter::HoldFeet()
{
	ServerHoldFeet();
}

void ARubberManCharacter::ServerHoldFeet_Implementation()
{
	MulticastHoldFeet();
}


bool ARubberManCharacter::ServerHoldFeet_Validate()
{
	return true;
}

void ARubberManCharacter::MulticastHoldFeet_Implementation()
{
	feet->keyPressed = true;
}


void ARubberManCharacter::ReleaseHands()
{
	ServerReleaseHands();
}

void ARubberManCharacter::ServerReleaseHands_Implementation()
{
	MulticastReleaseHands();
}

bool ARubberManCharacter::ServerReleaseHands_Validate()
{
	return true;
}

void ARubberManCharacter::MulticastReleaseHands_Implementation()
{
	hands->keyPressed = false;
}

void ARubberManCharacter::ReleaseFeet()
{
	ServerReleaseFeet();
}

void ARubberManCharacter::ServerReleaseFeet_Implementation()
{
	MulticastReleaseFeet();
}

bool ARubberManCharacter::ServerReleaseFeet_Validate()
{
	return true;
}

void ARubberManCharacter::MulticastReleaseFeet_Implementation()
{
	feet->keyPressed = false;
}

void ARubberManCharacter::SetControlMode(ControlMode mode)
{
	// Enable/disable all the components needed for the control modes to work properly
	this->mode = mode;
	bool defaultMode = true;
	if (mode == ControlMode::Elastic) defaultMode = false;
	// Alright, here we go...
	if (mode == ControlMode::Default)
	{
		// Sling shot effect
		// ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
		// The effect is created by blending the average velocities with the maximum velocities of the anchors
		// If the velocity difference is large, both anchors will have the same velocity (sling shot effect)
		// Else, the velocity will average, like any classical physical object colliding (restitution effect)
		float hVel = hands->GetComponentVelocity().Size();
		float fVel = feet->GetComponentVelocity().Size();
		FVector maxVel = (hVel > fVel) ? hands->GetComponentVelocity() : feet->GetComponentVelocity();
		FVector meanVel = (hands->GetComponentVelocity() + feet->GetComponentVelocity()) * 0.50f;
		float deltaVel = FMath::Abs(FMath::Abs(hVel) - FMath::Abs(fVel));
		float alphaMax = deltaVel / slingshotDeltaVelocity;
		if (alphaMax > 1) alphaMax = 1;
		FVector vel = (alphaMax * maxVel + (1 - alphaMax) * meanVel);

		// Unintelligible magic that seems to make things work properly
		// ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
		hands->SetSimulatePhysics(false);	// Deactivate the physics
		feet->SetSimulatePhysics(false);	// Deactivate the physics
		hands->SetHiddenInGame(true);		// Hide elastic
		feet->SetHiddenInGame(true);		// Hide elastic
		body->SetHiddenInGame(true);		// Hide elastic
		cam->AttachTo(GetCapsuleComponent());	// Fix camera
		cam->TargetArmLength = 300.0;			// Fix camera
		hands->SetWorldRotation(GetCapsuleComponent()->GetComponentToWorld().GetRotation().Rotator()); // Set the rotation of the anchors to the character rotation
		feet->SetWorldRotation(GetCapsuleComponent()->GetComponentToWorld().GetRotation().Rotator());  // Set the rotation of the anchors to the character rotation
		GetCharacterMovement()->SetActive(true); // Activate the movements
		GetMesh()->SetHiddenInGame(false);		 // Show full mesh
		hands->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// Disable hands collision
		feet->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// Disable feet collision
		body->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// Disable feet collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // Enable capsule physics
		body->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f)); // Deactivate body
		body->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));     // Deactivate body
		GetCapsuleComponent()->SetWorldLocation(feet->GetComponentLocation() - feetInitPosition);	// Update capsule position
		feet->SetWorldLocation(GetCapsuleComponent()->GetComponentLocation() + feetInitPosition);	// Update feet position
		hands->SetWorldLocation(GetCapsuleComponent()->GetComponentLocation() + handsInitPosition);	// Update hands position
		hands->AttachTo(GetCapsuleComponent());	// Re-attach hands (they get detached when the SetSimulatePhysics() method is called)
		feet->AttachTo(GetCapsuleComponent());	// Re-attach feet  (they get detached when the SetSimulatePhysics() method is called)
		hands->SetRelativeLocation(handsInitPosition); // Reset hands relative location (position in the blueprint template)
		feet->SetRelativeLocation(feetInitPosition);   // Reset feet relative location (position in the blueprint template)
		GetCharacterMovement()->BrakingDecelerationWalking = 0.0f;  // Friction prevents slingshot
		//GetCapsuleComponent()->SetSimulatePhysics(true); <--- LOLLLLLLLLLLLLLL

		// Momentum transfer
		// ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
		GetCapsuleComponent()->ComponentVelocity = vel;
		GetMovementComponent()->Velocity = vel;
		GetMovementComponent()->UpdateComponentVelocity();

		// Jump
		// ¯¯¯¯
		if (alphaMax > 0.75f)
			this->Jump();
	}
	else if (mode == ControlMode::Elastic)
	{
		hands->SetSimulatePhysics(true);	// Turn physics on
		feet->SetSimulatePhysics(true);		// Turn physics on
		hands->SetHiddenInGame(false);		// Show elastic
		feet->SetHiddenInGame(false);		// Show elastic
		body->SetHiddenInGame(false);		// Show elastic
		cam->AttachTo(body);				// Fix camera
		hands->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	// Turn collisions on
		feet->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	// Turn collisions on
		body->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	// Turn collisions on
		GetCharacterMovement()->SetActive(false);								  // Deactivate the movements
		GetMesh()->SetHiddenInGame(true);										  // Hide full mesh
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Disable capsule collision
		hands->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);  // This is necessary for some reasons...
		feet->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);   // This is necessary for some reasons...
		// TODO: body->SetSimulatePhysics(true);

		// Momentum transfer
		// Set the anchors velocity equal to the character velocity (the joints will take care of stopping the grabbing anchors)
		hands->SetAllPhysicsLinearVelocity(GetCapsuleComponent()->GetComponentVelocity());
		feet->SetAllPhysicsLinearVelocity(GetCapsuleComponent()->GetComponentVelocity());
	}
}
