// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "KidCharacter.generated.h"

//anim states 
UENUM(BlueprintType)
enum class EKidState : uint8
{
	None UMETA(DisplayName = "None"),
	Attacking UMETA(DisplayName = "Attacking"),
	Walking UMETA(DisplayName = "Walking"),
	Running UMETA(DisplayName = "Running"),
	Gliding UMETA(DisplayName = "Gliding")
};

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AKidCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AKidCharacter(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, Category = "AirMovement", meta = (UIMin = "0.0", UIMax = "1.0"))
	float BaseAirControl = 0.0f;

	UPROPERTY(EditAnywhere, Category = "AirMovement", meta = (UIMin = "0.0", UIMax = "1.0"))
	float BaseFallingLateralFriction = 0.5f;

	UPROPERTY(EditAnywhere, Category = "AirMovement", meta = (UIMin = "100.0", UIMax = "1000.0"))
	float GlidingMaxVerticalVelocity = 400.0f;

	UPROPERTY(EditAnywhere, Category = "AirMovement", meta = (UIMin = "0.0", UIMax = "1.0"))
	float GlidingAirControl = 0.85f;

	UPROPERTY(EditAnywhere, Category = "AirMovement", meta = (UIMin = "0.0", UIMax = "1.0"))
	float GlidingLateralFriction = 0.8f;

	UPROPERTY(BlueprintReadOnly, Category = "AirMovement")
	bool IsGliding = false;

	UPROPERTY(BlueprintReadOnly, Category = "AirMovement")
	EKidState CurrentAnimState;


	UFUNCTION(BlueprintCallable, Category = "Offence")
	bool CanAttack();


	UFUNCTION(reliable, Server, WithValidation)
		void ServerAttack();
	 
	UFUNCTION(reliable, NetMulticast)
		void MulticastAttack();


	//attacking animation
	UPROPERTY(EditDefaultsOnly, Category = GlidingAnimation)
		UAnimationAsset* AttackingAnimation;

	//attacking animation
	UPROPERTY(EditDefaultsOnly, Category = GlidingAnimation)
		UAnimationAsset* StartGlideAnimation;
	//attacking animation
	UPROPERTY(EditDefaultsOnly, Category = GlidingAnimation)
		UAnimationAsset* GlidingAnimation;
	//attacking animation
	UPROPERTY(EditDefaultsOnly, Category = GlidingAnimation)
		UAnimationAsset* StopGlideAnimation;

	/* Probably BlueprintAutorityOnly*/
	UFUNCTION(reliable, Server, WithValidation)
	void Glide();
	
	UFUNCTION(reliable, Server, WithValidation)
	void StopGliding();

	UFUNCTION()
	void Slash();

	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaSecond) override;

	void EnableGliding();
	void DisableGliding();
};