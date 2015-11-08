
#include "NNE.h"
#include "Net/UnrealNetwork.h"
#include "BaseCharacter.h"

ABaseCharacter::ABaseCharacter() : Super()
{
	bReplicates = true;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, HealthCurrent);
	DOREPLIFETIME(ABaseCharacter, HealthMax);
}

void ABaseCharacter::Die(){

	if (Controller && Role == ROLE_Authority) {
		/*
		AMainGameMode* mainGM = dynamic_cast<AMainGameMode*>(GetWorld()->GetAuthGameMode());

		if (mainGM)
		{
			mainGM->ACharacterHasDied(Controller);
		}
		*/
	}

	//Destroy();
}

float ABaseCharacter::GetCurrentHealth(){
	return HealthCurrent;
}
 
//Server validates health can be added, up to max defined by Health
//Then broadcasts the current health to all clients with ClientUpdateCurrentHealth()

bool ABaseCharacter::Heal_Validate(float HealAmount){

	return (Controller && Role == ROLE_Authority
		&& 0 <= HealAmount && HealthCurrent < HealthMax);
}

void ABaseCharacter::Heal_Implementation(float HealAmount){

	HealthCurrent += HealAmount;

	if (HealthCurrent > HealthMax) {
		HealthCurrent = HealthMax;
	}
}


bool ABaseCharacter::UseMain_Validate(){
	return true; // check if you have main weapon
}

void ABaseCharacter::UseMain_Implementation(){
	return;
}

bool ABaseCharacter::UseSecondary_Validate(){
	return true; // check if you have 2 weapons or 1 weapon with secondary fire
}

void ABaseCharacter::UseSecondary_Implementation(){
	return;
}

bool ABaseCharacter::UseAction_Validate(){
	return true;
}

void ABaseCharacter::UseAction_Implementation(){
	return;
}

//==============================================================================
// SHOULD BE CORRECTED 
//==============================================================================

// Non replicated for now
ABaseCharacter::Move(){
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}

	// Create fire direction vector
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

	// Try and fire a shot
	FireShot(FireDirection);
}

void ABaseCharacter::FireShot(FVector FireDirection)
{
	// If we it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				// spawn the projectile
				World->SpawnActor<ANNEProjectile>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ANNEPawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}