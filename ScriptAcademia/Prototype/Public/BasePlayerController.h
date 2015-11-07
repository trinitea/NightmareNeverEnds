// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CustomActionBindingComponent.h"
#include "BasePlayerController.generated.h"

/**
 *	Class for the base player controller. Has everything to define non-specific
 *	key bindings, third person camera, etc.
 */
UCLASS()
class PROTOTYPE_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

	
	//METHODS
public:
	//Constructor
	ABasePlayerController(const FObjectInitializer& ObjectInitializer);

	//Accessors
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetCamera() const { return ThirdPersonCamera; }

	// NETWORKED METHODS
public:
	UFUNCTION(reliable, client, BlueprintCallable, Category = Input)
		void BlockInput(EBindingAction Action);

	UFUNCTION(reliable, client, BlueprintCallable, Category = Input)
		void AllowInput(EBindingAction Action);

	UFUNCTION(BlueprintCallable, Category = "Character")
	int32 GetCharacterHealth();

protected:
	//Overriden methods
	virtual void BeginPlay() override;
	virtual void SetupInputComponent();
	virtual void Tick(float deltaTime);

private:
	//Input methods
	void MoveVertically(float value);
	void MoveHorizontally(float value);
	void RotateCameraHorizontally(float rate);
	void RotateCameraVertically(float rate);
	void Jump();
	void StopJumping();


	//VARIABLES
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		uint8 PlayerNumber;

private:
	//Camera boom for the third person camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//Third person camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ThirdPersonCamera;

	//If Y axis is inverted
	bool bInvertAxisY = false;

	//Camera horizontal rotation rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float CameraHorizontalRotationRate;

	//Camera vertical rotation rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float CameraVerticalRotationRate;

	// PROTECTED
protected:
	TMap<uint8, UCustomActionBindingComponent> InputBindings;

};
