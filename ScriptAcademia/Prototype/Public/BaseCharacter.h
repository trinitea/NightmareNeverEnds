#pragma once

#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

/*
 *	Class for the base character. Implement this class if you want to specialize
 *	a character.
 */
UCLASS()
class PROTOTYPE_API ABaseCharacter : public ACharacter
{

	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = "Death")
	virtual void Die();

	UFUNCTION(reliable, NetMulticast)
		virtual void OnHit(AActor *Other);


	UFUNCTION(BlueprintCallable, Category = "Damage")
		int32 GetAttackDamage();

	/** Damage dealt by character when attacking. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		int32 Damage = 55;
	
	//attack box to calculate attack range
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		FVector AttackBox = FVector(100.0f, 60.0f, 0.5f);


	/** The current health of the character.  Read only */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		int32 Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		int32 MaxHealth = 100;

	UFUNCTION(BlueprintCallable, Category = "Health")
		int32 GetCurrentHealth();

	//allows health to be added from BP events
	UFUNCTION(BlueprintCallable, Category = "Health")
		void AddHealth(int32 HealthToAdd);


	UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "On Health Changed"))
		virtual void OnHealthChanged(const int32& CurrentHealth);

protected:
	//When attacking, checks ennemy in ranges and applies damage only once
	void CheckAttackOverlap();

private:
	UFUNCTION(reliable, NetMulticast)
		void ApplyDamage(int32 dmg);

	//Server validates health can be added, up to max defined by Health
	//Then broadcasts the current health to all clients with ClientUpdateCurrentHealth()
	UFUNCTION(reliable, Server, WithValidation)
		void ApplyHealth(int32 HealthToAdd);

	UFUNCTION(reliable, NetMulticast)
		void ClientUpdateCurrentHealth(int32 NewHealthValue);
};
