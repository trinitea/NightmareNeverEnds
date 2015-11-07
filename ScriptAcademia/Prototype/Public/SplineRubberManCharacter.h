// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "SplineRubberManCharacter.generated.h"

/**
*
*/
UCLASS()
class PROTOTYPE_API ASplineRubberManCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:

	/* FROM PROTOTYPE CHARACTER */
	ASplineRubberManCharacter(const FObjectInitializer& ObjectInitializer);



	/** How fast will the character stretch. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterStretch)
		float BaseStretchRate = 50.0f;

	/** The minimum stretch ratio for the character. Should be 1.0f unless compressing the characted is required. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterStretch)
		float MinStretchRatio = 1.0f;

	/** The maximum stretch ratio for the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterStretch)
		float MaxStretchRatio = 2.0f;

	/** The current stretch ratio of the character.  Read only */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStretch)
		float CurrentStretchRatio = 1.0f;



	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ColliderCapsuleSize)
		float inRadius = 42.0f;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ColliderCapsuleSize)
		float inHalfHeight = 97.0f;


	virtual void BeginPlay() override;
	//void EndPlay();

	void StartStretch();
	void ReleaseStretch();

	bool CanAttachFeet();
	bool CanAttachHands();

	//Can stretch until maximum stretch ratio achieved.
	bool CanStretch();

	//Can unstretch until back to normal (minimum) stretch ratio.
	bool CanUnstretch();


protected:
	//Overridden methods
	virtual void PostInitializeComponents();

private:
	bool m_isStretching = false;
	virtual void Tick(float DeltaSecond) override;

 


};
