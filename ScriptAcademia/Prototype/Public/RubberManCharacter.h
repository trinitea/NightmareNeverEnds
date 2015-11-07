// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "Anchor.h"
#include "SquashBody.h"
#include "CameraSpringArmComponent.h"
#include "RubberManCharacter.generated.h"

UCLASS()
class ARubberManCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	ARubberManCharacter(const FObjectInitializer& ObjectInitializer);
public:
	void ARubberManCharacter::BeginPlay();
	void ARubberManCharacter::OnConstruction(const FTransform & transform);
	void ARubberManCharacter::Tick(float DeltaTime);

	UAnchor* hands;
	UAnchor* feet;
	FVector handsInitPosition;
	FVector feetInitPosition;
	USquashBody* body;
	USpringArmComponent* cam;

	float elasticity = 5.0f;
	float strength = 75.0f;
	float maxDist = 3000.0f;
	float slingshotDeltaVelocity = 1250.0f;

	void VStretch(float Value);
	UFUNCTION(reliable, NetMulticast)
		void MulticastVStretch(float Value);
	UFUNCTION(reliable, server, WithValidation)
		void ServerVStretch(float Value);
private:
	float forwardValue = 0.0f;
	float rightValue = 0.0f;
	float upValue = 0.0f;
	FVector forwardV;
	FVector rightV;
	FVector upV;
	void ElasticPhysics(FVector handsPos, FVector feetPos);
	void CalculateTension(FVector aPos, FVector bPos);


	void Stretch(UAnchor* controlledAnchor, UAnchor* grabbingAnchor);
	UFUNCTION(reliable, NetMulticast)
		void MulticastStretch(UAnchor* controlledAnchor, UAnchor* grabbingAnchor);
	UFUNCTION(reliable, server, WithValidation)
		void ServerStretch(UAnchor* controlledAnchor, UAnchor* grabbingAnchor);

	enum ControlMode { Default, Elastic };
	void SetControlMode(ControlMode mode);

	ControlMode mode = ControlMode::Default;
	float naturalLength = 1.0f;
	float handsHeight = 1.0f;
	float feetHeight = 1.0f;
	float tension = 0.0f;

	// Default third person character
public:
	void HoldHands();
	UFUNCTION(reliable, NetMulticast)
		void MulticastHoldHands();
	UFUNCTION(reliable, server, WithValidation)
		void ServerHoldHands();

	void HoldFeet();
	UFUNCTION(reliable, NetMulticast)
		void MulticastHoldFeet();
	UFUNCTION(reliable, server, WithValidation)
		void ServerHoldFeet();

	void ReleaseHands();
	UFUNCTION(reliable, NetMulticast)
		void MulticastReleaseHands();
	UFUNCTION(reliable, server, WithValidation)
		void ServerReleaseHands();

	void ReleaseFeet();
	UFUNCTION(reliable, NetMulticast)
		void MulticastReleaseFeet();
	UFUNCTION(reliable, server, WithValidation)
		void ServerReleaseFeet();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	void MoveForward(float Value);
	UFUNCTION(reliable, NetMulticast)
		void MulticastMoveForward(float Value);
	UFUNCTION(reliable, server, WithValidation)
		void ServerMoveForward(float Value);

	void MoveRight(float Value);
	UFUNCTION(reliable, NetMulticast)
		void MulticastMoveRight(float Value);
	UFUNCTION(reliable, server, WithValidation)
		void ServerMoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};