#pragma once

#include "GameFramework/Character.h"
#include "Interface/WieldableInterface.h"
#include "BaseCharacter.generated.h"

UCLASS(Blueprintable)
class ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

//==============================================================================
// PROPERTIES
//==============================================================================

	/* The mesh component (for testing. some other meshed can be use later)*/
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf <class ANNEProjectile> ProjectileBP;
	
	UPROPERTY(BlueprintReadWrite, Category = "Movement") // might be replicated as well
	float MoveSpeed = 1000.0f;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	FVector GunOffset;

	/*
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Health")
	float HealthMax = 100.0f;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Health")
	float HealthCurrent = 100.0f;

	

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Weapon")
	TScriptInterface<IWieldable> MainHandItem;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Weapon")
	TScriptInterface<IWieldable> OffHandItem;
	*/
private:
	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

//==============================================================================
// FUNCTIONS
//==============================================================================
public:

	UFUNCTION(reliable, Server, WithValidation, Blueprintable, Category = "Movement")
	void Move(const float ForwardValue, const float RightValue, const float DeltaSeconds, const FRotator DirectionPerspective);

	UFUNCTION(reliable, Server, WithValidation, Blueprintable, Category = "Movement")
	void Aim(const float ForwardValue, const float RightValue, const FRotator DirectionPerspective);
	
	UFUNCTION(reliable, Server, WithValidation, Blueprintable, Category = "Action")
	void UseMain();

	UFUNCTION(reliable, Server, WithValidation, Blueprintable, Category = "Action")
	void UseSecondary();

	/*
	UFUNCTION(reliable, Server, WithValidation, Blueprintable, Category = "Action")
	void UseAction();
	
	UFUNCTION(Blueprintable, Category = "Health")
	float GetCurrentHealth();

	UFUNCTION(reliable, Server, WithValidation, Blueprintable, Category = "Health")
	void Heal(float HealthAmount);

	// notify the UI
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Health Changed"))
	void OnHealthChanged(const float& Health);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Die();
	*/


	void ShotTimerExpired();
	/** Returns ShipMeshComponent subobject (not sure here)**/
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
};

