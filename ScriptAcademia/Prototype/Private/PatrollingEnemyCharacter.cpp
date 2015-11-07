// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "Engine.h"
#include "PatrollingAIController.h"
#include "PatrollingEnemyCharacter.h"
#include "BaseCharacter.h"

/*
 *	Default constructor.
 */
APatrollingEnemyCharacter::APatrollingEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<AAIController> AIControllerBPClass(TEXT("Blueprint'/Game/Blueprints/PatrollingAIControllerBP'"));
	if (AIControllerBPClass.Class != NULL)
	{
		AIControllerClass = AIControllerBPClass.Class;
	}
	PerceptionMesh = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "Box1");
	PerceptionMesh->AttachParent = RootComponent;
	PerceptionMesh->OnComponentBeginOverlap.AddDynamic(this, &APatrollingEnemyCharacter::OnOverlapBegin);
	PerceptionMesh->OnComponentEndOverlap.AddDynamic(this, &APatrollingEnemyCharacter::OnOverlapEnd);
}


/*
*	On Overlap Implementation.
*/
void APatrollingEnemyCharacter::OnOverlapBegin_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!dynamic_cast<APatrollingAIController*>(GetController())) return;

	((APatrollingAIController*)GetController())->OnOverlapBegin_Implementation(OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

/*
*	On Overlap End Implementation.
*/
void APatrollingEnemyCharacter::OnOverlapEnd_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!dynamic_cast<APatrollingAIController*>(GetController())) return;

	((APatrollingAIController*)GetController())->OnOverlapEnd_Implementation(OtherActor, OtherComp, OtherBodyIndex);
}


/*
 *	Launch attack and set material.
 */
bool APatrollingEnemyCharacter::Attack(AActor* Target)
{
	// If is not authority, no need to execute this
	if (Role != ROLE_Authority) return false;
	
	// If is not base character, stop
	if (!dynamic_cast<ABaseCharacter*>(Target)) return false;

	// If player has no more health, stop attack
	if (((ABaseCharacter*)Target)->GetCurrentHealth() <= 0)
	{
		AttackState = EAttackState::None;
		return true;
	}

	switch (AttackState)
	{
		// If we have not started the attack, start it
	case EAttackState::None:
		MulticastAttack(Target);
		break;

		// If attack is finished, reset attack state and return true
	case EAttackState::FinishedAttack:
		AttackState = EAttackState::None;
		break;
	default:
		break;
	}
	// Return false if we get here
	return false;
}

/*
 *	Start Attack Mode.
 */
void APatrollingEnemyCharacter::StartAttackMode()
{
	if (Role != ROLE_Authority) return;

	MulticastStartAttack();
}

/*
 *	Start Searching and set material.
 */
void APatrollingEnemyCharacter::StartSearchMode()
{
	if (Role != ROLE_Authority) return;

	MulticastSearch();
}

/*
*	Start Patrolling and set material.
*/
void APatrollingEnemyCharacter::StartPatrolMode()
{
	if (Role != ROLE_Authority) return;

	MulticastPatrol();
}


/*
*	Begin play override.
*/
void APatrollingEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set material
	GetMesh()->SetMaterial(0, DefaultMaterial);
}

/*
 *	Tick override event.
 */
void APatrollingEnemyCharacter::Tick(float DeltaSeconds)
{
	switch (AttackState)
	{
		// If is not attacking, do nothing
	case EAttackState::None:
	case EAttackState::FinishedAttack:
		return;
		break;
		
		// Wait for Attack, increment wait time and trigger attack once reached 
	case EAttackState::WaitingForAttack:
		AttackWaitTime += DeltaSeconds;
		
		if (AttackWaitTime > AttackWaitDuration)
		{
			AttackWaitTime = 0.f;
			AttackState = EAttackState::Attacking;
		}
		break;

		// Attacking, increment anim time and trigger finished when reached
	case EAttackState::Attacking:
		AttackAnimationTime += DeltaSeconds;
		
		if (AttackAnimationTime > AttackAnimationDuration)
		{
			// Set Attack finished
			AttackAnimationTime = 0.f;
			AttackState = EAttackState::FinishedAttack;
			
			// TODO: REMOVE IN FINAL VERSION
			FString target = (PlayerTarget) ? PlayerTarget->GetName() : "No Target";
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Finished Attack " + target);

			// If on server, target set and in range, trigger on hit
			if (Role == ROLE_Authority 
				&& PlayerTarget != NULL
				&& (RootComponent->GetComponentLocation() - PlayerTarget->GetRootComponent()->GetComponentLocation()).Size() < DistanceFromTargetForAttack)
			{
				PlayerTarget->OnHit(this);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Actor " + PlayerTarget->GetName() + " is Hit");
			}
		}
		break;
	}
}


/*
 *	Implementation of the Attack validation. Returns false if the 
 *	target is null.
 */
bool APatrollingEnemyCharacter::MulticastAttack_Validate(AActor* Target)
{
	return (Target != NULL);
}

/*
 *	Networked attack method. Sets the character's material.
 */
void APatrollingEnemyCharacter::MulticastAttack_Implementation(AActor* Target)
{
	//If already attacking, do nothing
	if (AttackState != EAttackState::None) return;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Start Attack");
	AttackState = EAttackState::Attacking;

	if (Role != ROLE_Authority || !dynamic_cast<ABaseCharacter*>(Target)) return;

	PlayerTarget = (ABaseCharacter*) Target;
}

/*
 *	Sets the character's material.
 */
void APatrollingEnemyCharacter::MulticastStartAttack_Implementation()
{
	GetMesh()->SetMaterial(0, AttackingMaterial);
}

/*
*	Networked search method. Sets the character's material.
*/
void APatrollingEnemyCharacter::MulticastSearch_Implementation()
{
	GetMesh()->SetMaterial(0, SearchingMaterial);
}

/*
*	Networked patrol method. Sets the character's material.
*/
void APatrollingEnemyCharacter::MulticastPatrol_Implementation()
{
	GetMesh()->SetMaterial(0, DefaultMaterial);
}



void APatrollingEnemyCharacter::Die_Implementation()
{
	if (Role == ROLE_Authority) {
		Destroy();
	}
}



//Character has been hit by enemy 
void APatrollingEnemyCharacter::OnHit_Implementation(AActor *Other)
{
	//server event 
	if (Role == ROLE_Authority) {

		//apply damage received from the other Character
		if (dynamic_cast<ABaseCharacter*>(Other)){
			ApplyDamage(Cast<ABaseCharacter>(Other)->GetAttackDamage());
		}
	}
}



//Apply reveived damage on the character 
void APatrollingEnemyCharacter::ApplyDamage_Implementation(int32 dmg){

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Ennemy Hit");

	Health -= dmg;
	if (Health <= 0){
		Die();
	}
}

int32 APatrollingEnemyCharacter::GetCurrentHealth(){
	return Health;
}

int32 APatrollingEnemyCharacter::GetAttackDamage(){
	return Damage;
}
