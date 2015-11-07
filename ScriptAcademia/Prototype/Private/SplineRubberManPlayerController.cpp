#include "Prototype.h"
#include "SplineRubberManCharacter.h"
#include "SplineRubberManPlayerController.h"

 


ASplineRubberManPlayerController::ASplineRubberManPlayerController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}



void ASplineRubberManPlayerController::SetupInputComponent()
{

	//check performed in base class
	Super::SetupInputComponent();  //(InputComponent);

	InputComponent->BindAction("left_trigger_shoulder", IE_Pressed, this, &ASplineRubberManPlayerController::StartStretch);		//DEBUG: left shift
	InputComponent->BindAction("left_trigger_shoulder", IE_Released, this, &ASplineRubberManPlayerController::ReleaseStretch);

	//InputComponent->BindAxis("left_trigger_shoulder",   this, &ARubberManCharacter::StartStretch);
	//InputComponent->BindAxis("right_trigger_shoulder", this, &ARubberManCharacter::ReleaseStretch);

}


/*
*	Method that starts the stretch.
*/
void ASplineRubberManPlayerController::StartStretch()
{
	//Do nothing if no pawn or not ruberman character
	if (GetPawn() == NULL || !dynamic_cast<ASplineRubberManPlayerController*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Spline Rubber Man Pawn defined"));
		return;
	}

	((ASplineRubberManPlayerController*)GetPawn())->StartStretch();
}

/*
*	Method that releases the stretch.
*/
void ASplineRubberManPlayerController::ReleaseStretch()
{
	//Do nothing if no pawn or not ruberman character
	if (GetPawn() == NULL || !dynamic_cast<ASplineRubberManPlayerController*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Spline Rubber Man Pawn defined"));
		return;
	}

	((ASplineRubberManPlayerController*)GetPawn())->ReleaseStretch();
}