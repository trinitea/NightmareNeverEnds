// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "KidCharacter.h"
#include "Engine.h"

AKidCharacter::AKidCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AKidCharacter::BeginPlay() {
	GetCharacterMovement()->AirControl = BaseAirControl;
	GetCharacterMovement()->FallingLateralFriction = BaseFallingLateralFriction;
}

bool AKidCharacter::Glide_Validate()
{
	return true; //GetCharacterMovement()->IsFalling();
}

void AKidCharacter::Glide_Implementation()
{
	EnableGliding();
}

bool AKidCharacter::StopGliding_Validate()
{
	return true;
}

void AKidCharacter::EnableGliding(){
	
	if (GetCharacterMovement()->IsFalling()){
		IsGliding = true;
		GetCharacterMovement()->AirControl = GlidingAirControl;
		GetCharacterMovement()->FallingLateralFriction = GlidingLateralFriction;

		//set animation start 
		if (StartGlideAnimation)
		{
			UE_LOG(LogTemp, Error, TEXT("Kid Playing StartGlideAnimation"));
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			GetMesh()->PlayAnimation(StartGlideAnimation, false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Kid StartGlideAnimation is null"));
		}
	}
}

void AKidCharacter::StopGliding_Implementation()
{
	DisableGliding();
}

void AKidCharacter::DisableGliding()
{
	if (IsGliding){
		GetCharacterMovement()->AirControl = BaseAirControl;
		GetCharacterMovement()->FallingLateralFriction = BaseFallingLateralFriction;


		//set animation end 
		//set animation start 
		if (StopGlideAnimation)
		{
			UE_LOG(LogTemp, Error, TEXT("Kid Playing StopGlideAnimation"));
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			GetMesh()->PlayAnimation(StopGlideAnimation, false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Kid StopGlideAnimation is null"));
		}
	}
	IsGliding = false;
}

void AKidCharacter::Slash()
{
	if (CanAttack()) {

		//Can't launch another attack if current one has not been completed.
		if (CurrentAnimState != EKidState::Attacking){
			ServerAttack();
			
		}
	}
}

void AKidCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetMesh()->GetAnimationMode() == EAnimationMode::AnimationSingleNode)
	{
		if (GetMesh()->GetSingleNodeInstance()->CurrentAsset == AttackingAnimation
			&& !GetMesh()->GetSingleNodeInstance()->bPlaying)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			CurrentAnimState = EKidState::None;
		}
		else if (GetMesh()->GetSingleNodeInstance()->CurrentAsset == StartGlideAnimation
			 && !GetMesh()->GetSingleNodeInstance()->bPlaying)
		{
			//stay in GlideLoop animation
			CurrentAnimState = EKidState::Gliding;
			GetMesh()->PlayAnimation(GlidingAnimation, true);
		}

		//else, glide loop until stopped in controller

		else if (GetMesh()->GetSingleNodeInstance()->CurrentAsset == StopGlideAnimation
			&& !GetMesh()->GetSingleNodeInstance()->bPlaying)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			CurrentAnimState = EKidState::None;
		}
	}

	if (IsGliding) {
		if (!GetCharacterMovement()->IsFalling()){
			DisableGliding();
		}
		else
		{
			if (Role == ROLE_Authority) {
				if (GetCharacterMovement()->Velocity.Z < -GlidingMaxVerticalVelocity || GetCharacterMovement()->Velocity.Z > GlidingMaxVerticalVelocity){
					FVector vec = GetCharacterMovement()->Velocity * FVector(0, 0, -400.00f) * GetCharacterMovement()->Mass;
					GetCharacterMovement()->AddForce(DeltaSeconds * vec);
				}
			}
		}
		
	}
}

bool AKidCharacter::CanAttack()
{
	return !GetCharacterMovement()->IsFalling();
}



//Notes on attacking: 
//When weapon attack volume overlaps with  actor, will trigger onHit and validate it was an ennemy.    
//Also, onHit() only triggers on overlap AND if CurrentAnimState is attacking. 
 
 
/*
*	Implementation of the Attack validation. 
*/
bool AKidCharacter::ServerAttack_Validate()
{
	//return (Target != NULL);

	return CurrentAnimState != EKidState::Attacking;
}

/*
*	Networked attack method. Sets the character's material.
*/
void AKidCharacter::ServerAttack_Implementation()
{
	 
	if (Role == ROLE_Authority)
	{
		MulticastAttack();
	}

	 
} 
 

/*
*	Networked attack method. Sets the character's material.
*/
void AKidCharacter::MulticastAttack_Implementation()
{
 
	CurrentAnimState = EKidState::Attacking; 

	if (AttackingAnimation)
	{
		UE_LOG(LogTemp, Error, TEXT("Kid Playing attacking animation"));
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->PlayAnimation(AttackingAnimation, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Kid attacking Animation is null"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "KID Slash Attack");
	}

	
	
	CheckAttackOverlap();

	//TODO: when anim ends, release CurrentAnimState.
	
}


