// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "KidPlayerController.h"
#include "KidCharacter.h"

AKidPlayerController::AKidPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AKidPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//GAME PAD MAPPINGS
	TArray<FCustomBindingData> bindings;

	bindings.Add(FCustomBindingData(InputComponent->BindAxis("right_trigger_axis", this, &AKidPlayerController::GlideManager)));

	InputBindings.Add(GetEnumValue(EBindingAction::Glide), UCustomActionBindingComponent(bindings));
	
	//If an action is only binded dynamically (and not for the start) use this
	//InputBindings.Add(GetEnumValue(EBindingAction::Glide), UCustomActionBindingComponent(bindings, false, InputComponent));

	InputBindings.Add(GetEnumValue(EBindingAction::Slash), UCustomActionBindingComponent(InputComponent->BindAction("x_button", IE_Pressed, this, &AKidPlayerController::Slash)));

	// LOCK ACTIONS WE WANT TO BE UNAVAILABLE ON START COMMENTED FOR NOW
	//InputBindings.Find(GetEnumValue(EBindingAction::Glide))->UnbindAction(InputComponent);
	//InputBindings.Find(GetEnumValue(EBindingAction::Slash))->UnbindAction(InputComponent);
}

void AKidPlayerController::GlideManager(float value){

	//Do nothing if no pawn or not character
	if (GetPawn() == NULL || !dynamic_cast<AKidCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	if (bGlideTrigger) {
		
		if (value < TriggerPressureThreshold) {
			((AKidCharacter*)GetPawn())->StopGliding();
			bGlideTrigger = false;
		}
	}
	else {
		if (value > TriggerPressureThreshold) {
			((AKidCharacter*)GetPawn())->Glide();
			bGlideTrigger = true;
		}
	}
}

/*
*	Attack !!!
*/
void AKidPlayerController::Slash()
{
	//Do nothing if no pawn or not character
	if (GetPawn() == NULL || !dynamic_cast<AKidCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn defined"));
		return;
	}

	((AKidCharacter*)GetPawn())->Slash();
}


