// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "RubberManPlayerController.h"
#include "RubberManCharacter.h"

ARubberManPlayerController::ARubberManPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void ARubberManPlayerController::SetupInputComponent()
{
	// Set up gameplay key bindings
	Super::SetupInputComponent();
	InputComponent->BindAction("right_trigger", IE_Pressed, this, &ARubberManPlayerController::HoldHands);
	InputComponent->BindAction("left_trigger", IE_Pressed, this, &ARubberManPlayerController::HoldFeet);
	InputComponent->BindAction("right_trigger", IE_Released, this, &ARubberManPlayerController::ReleaseHands);
	InputComponent->BindAction("left_trigger", IE_Released, this, &ARubberManPlayerController::ReleaseFeet);

	InputComponent->BindAxis("leftstick_vertical", this, &ARubberManPlayerController::MoveForward);
	InputComponent->BindAxis("leftstick_horizontal", this, &ARubberManPlayerController::MoveRight);
	InputComponent->BindAxis("vStretch", this, &ARubberManPlayerController::VStretch);
}

void ARubberManPlayerController::HoldHands()
{
	if (GetPawn() == NULL || !dynamic_cast<ARubberManCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	((ARubberManCharacter*)(GetPawn()))->HoldHands();
}
void ARubberManPlayerController::HoldFeet()
{
	if (GetPawn() == NULL || !dynamic_cast<ARubberManCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	((ARubberManCharacter*)(GetPawn()))->HoldFeet();
}
void ARubberManPlayerController::ReleaseHands()
{
	if (GetPawn() == NULL || !dynamic_cast<ARubberManCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	((ARubberManCharacter*)(GetPawn()))->ReleaseHands();
}
void ARubberManPlayerController::ReleaseFeet()
{
	if (GetPawn() == NULL || !dynamic_cast<ARubberManCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	((ARubberManCharacter*)(GetPawn()))->ReleaseFeet();
}
void ARubberManPlayerController::VStretch(float Value)
{
	if (GetPawn() == NULL || !dynamic_cast<ARubberManCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	((ARubberManCharacter*)(GetPawn()))->VStretch(Value);
}
void ARubberManPlayerController::MoveForward(float Value)
{
	if (GetPawn() == NULL || !dynamic_cast<ARubberManCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	((ARubberManCharacter*)(GetPawn()))->MoveForward(Value);
}
void ARubberManPlayerController::MoveRight(float Value)
{
	if (GetPawn() == NULL || !dynamic_cast<ARubberManCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	((ARubberManCharacter*)(GetPawn()))->MoveRight(Value);
}
void ARubberManPlayerController::TurnAtRate(float Rate)
{
	if (GetPawn() == NULL || !dynamic_cast<ARubberManCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	((ARubberManCharacter*)(GetPawn()))->TurnAtRate(Rate);
}
void ARubberManPlayerController::LookUpAtRate(float Rate)
{
	if (GetPawn() == NULL || !dynamic_cast<ARubberManCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	((ARubberManCharacter*)(GetPawn()))->LookUpAtRate(Rate);
}