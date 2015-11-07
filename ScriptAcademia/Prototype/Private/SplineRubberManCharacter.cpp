// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "SplineRubberManCharacter.h"



ASplineRubberManCharacter::ASplineRubberManCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}


/*
*	Overriden Post Initialize Components to get mesh references.
*/
void ASplineRubberManCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	 

}


void ASplineRubberManCharacter::BeginPlay() {
 


}
 

/*
*  starts stretching up in tick()
*/
void ASplineRubberManCharacter::StartStretch()
{
	m_isStretching = true;

}

/*
*  disable stretching up in tick()
*/
void ASplineRubberManCharacter::ReleaseStretch()
{
	m_isStretching = false;
}


/*
*  Allows stretching up to occur in tick() if currently under MaxStretchRatio
*/
bool ASplineRubberManCharacter::CanStretch()
{
	return (CurrentStretchRatio < MaxStretchRatio);
}

/*
*  Allows stretching down to occur in tick() if currently above MinStretchRatio
*/
bool ASplineRubberManCharacter::CanUnstretch()
{
	return(CurrentStretchRatio > MinStretchRatio);
}

/*
*   
*  TODO:  
*/
bool ASplineRubberManCharacter::CanAttachFeet()
{
	return false;
}

/*
*
*  TODO:
*/
bool ASplineRubberManCharacter::CanAttachHands()
{
	return false;
}



void ASplineRubberManCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (m_isStretching) {

		if (CanStretch())
		{
 


			//GetCapsuleComponent()->SetCapsuleSize(inRadius, inHalfHeight * CurrentStretchRatio);
		}

	}
	else   //only unstrectch if not stretching
	{
		//and not back to normal
		if (CanUnstretch())
		{
 
			//GetCapsuleComponent()->SetCapsuleSize(inRadius, inHalfHeight * CurrentStretchRatio);
		}
	}
}
