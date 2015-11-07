// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "PatrollingEnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EAttackState : uint8
{
	None UMETA(DisplayName = "None"),
	WaitingForAttack UMETA(DisplayName = "WaitingForAttack"),
	Attacking UMETA(DisplayName = "Attacking"),
	FinishedAttack UMETA(DisplayName = "FinishedAttack")
};

/*
 *	Method to print the current state, useful for debugging.
 */
FORCEINLINE FString ToString(EAttackState enumVal)
{
	switch (enumVal)
	{
	case EAttackState::None:
		return "None";
	case EAttackState::WaitingForAttack:
		return "Waiting For Attack";
	case EAttackState::Attacking:
		return "Attacking";
	case EAttackState::FinishedAttack:
		return "Finished Attack";
	}
	return "";
}


/**
 * 
 */
UCLASS()
class PROTOTYPE_API APatrollingEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//Constructor
	APatrollingEnemyCharacter(const FObjectInitializer& ObjectInitializer);

	//Accessors
	FORCEINLINE TArray<AActor*> GetPatrollingNodes() const { return PatrollingNodes; }


	// ACTOR OVERLAP FUNCTIONS
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Switch Functions")
		void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category = "Switch Functions")
		void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// PUBLIC FUNCTIONS
public:
	UFUNCTION(BlueprintCallable, Category = "Damage")
		int32 GetAttackDamage();
	UFUNCTION(BlueprintCallable, Category = "Health")
		int32 GetCurrentHealth();
	bool Attack(AActor* Target);
	void StartAttackMode();
	void StartSearchMode();
	void StartPatrolMode();
	

	// OVERRIDEN FUNCTIONS
private:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);

	// NETWORKED FUNCTIONS
private:
	UFUNCTION(reliable, NetMulticast, WithValidation)
		void MulticastAttack(AActor* Target);
	UFUNCTION(reliable, NetMulticast)
		void MulticastStartAttack();
	UFUNCTION(reliable, NetMulticast)
		void MulticastSearch();
	UFUNCTION(reliable, NetMulticast)
		void MulticastPatrol();
	UFUNCTION(reliable, NetMulticast)
		void ApplyDamage(int32 dmg);
public:
	UFUNCTION(reliable, NetMulticast)
		virtual void OnHit(AActor *Other);
	UFUNCTION(reliable, NetMulticast, BlueprintCallable, Category = "Death")
		virtual void Die();


	// VARIABLES
public:
	// Materials
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = StateMaterials)
		UMaterialInstance* DefaultMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = StateMaterials)
		UMaterialInstance* SearchingMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = StateMaterials)
		UMaterialInstance* AttackingMaterial;
	
	/** The current health of the character.  Read only */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
		int32 Health = 100;


private:

	//List of patrol nodes
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Controller, meta = (AllowPrivateAccess = "true"))
		TArray<AActor*> PatrollingNodes;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AiPerceptionComponents, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* PerceptionMesh;

	// EXPOSED ATTACK VARIABLES
public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackOptions)
		float AttackWaitDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackOptions)
		float AttackAnimationDuration = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackOptions)
		float DistanceFromTargetForAttack = 150.f;

	/** Damage dealt by character when attacking. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackOptions)
		int32 Damage = 25;

	// ATTACK VARIABLES
private:
	EAttackState AttackState = EAttackState::None;
	ABaseCharacter* PlayerTarget = NULL;

	float AttackWaitTime = 0.f;
	float AttackAnimationTime = 0.f;
};
