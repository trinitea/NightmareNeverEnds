// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "PatrollingEnemyCharacter.h"
#include "RubberManCharacter.h"
#include "BaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrollingAIController.h"


/*
 *	Default Constructor.
 */
APatrollingAIController::APatrollingAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//Initialize blackboard
	Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
	
	//Attach AI controller to pawn
	bAttachToPawn = true;
}


/*
 *	Initialize patrolling mode by setting the base values.
 */
void APatrollingAIController::StartPatrollingMode()
{
	EState LastState = CurrentState;

	// Find Nearest Node
	CurrentPatrolNodeIndex = GetIndexOfNearestNode();
	
	// If node is not valid, stop
	if(CurrentPatrolNodeIndex < 0) return;
	
	CurrentState = EState::Patrolling;
	Blackboard->SetValueAsEnum(CurrentStateName, (uint8) CurrentState);
		
	CurrentPatrolNode = PatrollingNodes[CurrentPatrolNodeIndex];
	Blackboard->SetValueAsObject(CurrentPatrolNodeName, CurrentPatrolNode);

	if (!GetPawn() || !dynamic_cast<APatrollingEnemyCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("Controlled Pawn missing!"));
		return;
	}
	((APatrollingEnemyCharacter*)GetPawn())->StartPatrolMode();

	// End last state
	EndState(LastState);
}

/*
 *	Initialize attacking mode by setting the base values.
 */
void APatrollingAIController::StartAttackingMode()
{
	EState LastState = CurrentState;
	
	CurrentState = EState::Attacking;
	Blackboard->SetValueAsEnum(CurrentStateName, (uint8)CurrentState);

	if (!GetPawn() || !dynamic_cast<APatrollingEnemyCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("Controlled Pawn missing!"));
		return;
	}
	((APatrollingEnemyCharacter*)GetPawn())->StartAttackMode();

	EndState(LastState);
}

/*
 *	Initialize searching mode by setting the base values.
 */
void APatrollingAIController::StartSearchingMode()
{
	EState LastState = CurrentState;
	
	CurrentState = EState::Searching;
	Blackboard->SetValueAsEnum(CurrentStateName, (uint8)CurrentState);
	
	if (!GetPawn() || !dynamic_cast<APatrollingEnemyCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("Controlled Pawn missing!"));
		return;
	}
	((APatrollingEnemyCharacter*)GetPawn())->StartSearchMode();
	
	bIsSearchFinished = false;
	EndState(LastState);
}

/*
 *	Launch attack.
 */
bool APatrollingAIController::Attack()
{
	if (!dynamic_cast<APatrollingEnemyCharacter*>(GetPawn())) return true;

	bool bTargetDead = ((APatrollingEnemyCharacter*)GetPawn())->Attack(CurrentPlayerTarget);

	if (bTargetDead)
	{
		CurrentPlayerTarget = NULL;
		UpdatePlayerTarget();
		Blackboard->SetValueAsObject(CurrentPlayerTargetName, CurrentPlayerTarget);
	}

	return bTargetDead;
}

/*
 *	Launch search, returns true if finished.
 */
bool APatrollingAIController::Search()
{
	// If search is finished, notify the task
	if (bIsSearchFinished) return true;

	// If we are not searching, initialize variables
	if (!bIsSearching)
	{
		TotalSearchTime = 0.f;
		bIsSearching = true;
	}
	return false;
}


/*
*	Collision overlap event. Checks if the object in the field of view and the type of the object
*	and if it is a target, set it.
*/
void APatrollingAIController::OnOverlapBegin_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*
	*	If the current target is rubberman, we don't need to evaluate, and if the
	*	Other actor is not a base character, ignore. If other actor is current player
	*	ignore too.
	*/
	if (!OtherActor
		|| !dynamic_cast<ABaseCharacter*>(OtherActor)) return;

	// If does not contain actor add in InRangeList
	if (!ActorsInRange.Contains(OtherActor))
	{
		ActorsInRange.Add(OtherActor);
	}

	if (bPlayerTargetVisible && dynamic_cast<ARubberManCharacter*>(CurrentPlayerTarget)) return;

	ABaseCharacter* TempPlayerTarget = (ABaseCharacter*)OtherActor;

	// If temp target is not visible check current target
	if (!IsActorVisible<ABaseCharacter>(TempPlayerTarget))
	{
		// If the current target is null, set it to temp player else it will stay null
		if (!CurrentPlayerTarget)
		{
			CurrentPlayerTarget = TempPlayerTarget;
			Blackboard->SetValueAsObject(CurrentPlayerTargetName, CurrentPlayerTarget);
		}

		// Check if target visible
		bool Temp = IsActorVisible<ABaseCharacter>(CurrentPlayerTarget);

		// Update visibility to blackboard if different
		if (Temp != bPlayerTargetVisible)
		{
			bPlayerTargetVisible = Temp;
			Blackboard->SetValueAsBool(bPlayerTargetVisibleName, bPlayerTargetVisible);
		}
	}
	else
	{
		// Set blackboard Values
		CurrentPlayerTarget = (ABaseCharacter*)OtherActor;
		Blackboard->SetValueAsObject(CurrentPlayerTargetName, CurrentPlayerTarget);
		bPlayerTargetVisible = true;
		Blackboard->SetValueAsBool(bPlayerTargetVisibleName, bPlayerTargetVisible);
	}
}

/*
*	On Overlap end called. Used to keep track of the actors in the perception mesh.
*/
void APatrollingAIController::OnOverlapEnd_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor
		|| !dynamic_cast<ABaseCharacter*>(OtherActor)) return;

	// If contains actor remove in InRangeList
	if (ActorsInRange.Contains(OtherActor))
	{
		ActorsInRange.Remove(OtherActor);
	}
}


/*
 *	Begin play override.
 */
void APatrollingAIController::BeginPlay()
{
	Super::BeginPlay();

	Possess(GetPawn());

	// Check if one of the AI Behavior is missing
	if (!BehaviorTree || !BlackboardData || !Blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("Blackboard component, blackboard data or behavior tree is null!"));
		return;
	}
	
	// Initialize Blackboard
	Blackboard->InitializeBlackboard(BlackboardData);
	Blackboard->RegisterComponent();

	// Launch blackboard and behavior tree
	UseBlackboard(BlackboardData);
	RunBehaviorTree(BehaviorTree);

	if (!GetPawn() || !dynamic_cast<APatrollingEnemyCharacter*>(GetPawn()))
	{
		UE_LOG(LogTemp, Error, TEXT("Controlled Pawn missing!"));
		return;
	}

	//Get patrolling nodes
	PatrollingNodes = ((APatrollingEnemyCharacter*)GetPawn())->GetPatrollingNodes();

	//Check if has nodes
	if (PatrollingNodes.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No patrolling nodes"));
		return;
	}

	// Setup Current patrol node
	CurrentPatrolNodeIndex = GetIndexOfNearestNode();
	CurrentPatrolNode = (CurrentPatrolNodeIndex < 0) ? NULL : PatrollingNodes[CurrentPatrolNodeIndex];
	Blackboard->SetValueAsObject(CurrentPatrolNodeName, CurrentPatrolNode);
	
	// Set Black board values
	Blackboard->SetValueAsFloat(DistanceFromLastKnownPositionName, DistanceFromLastKnownPosition);
	Blackboard->SetValueAsFloat(DistanceFromPlayerTargetName, DistanceFromPlayerTarget);
	Blackboard->SetValueAsEnum(CurrentStateName, (uint8)CurrentState);
}

/*
 *	Override for the Tick event.
 */
void APatrollingAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/* 
	 *	Update player target to check if we have the best one, or if
	 *	a target already in range becomes visible.
	 */
	UpdatePlayerTarget();

	switch (CurrentState)
	{
	case EState::Attacking:
		
		if (!CurrentPlayerTarget) return;

		// If player is not visible, set last known position
		if (!bPlayerTargetVisible)
		{
			LastKnownPosition = CurrentPlayerTarget->GetRootComponent()->GetComponentLocation();
			Blackboard->SetValueAsVector(LastKnownPositionName, LastKnownPosition);
		}
		
		// Calculate distance from player target
		DistanceFromPlayerTarget = (CurrentPlayerTarget->GetActorLocation() - RootComponent->GetComponentLocation()).Size();
		Blackboard->SetValueAsFloat(DistanceFromPlayerTargetName, DistanceFromPlayerTarget);
		break;
	case EState::Searching:
		// Calculate distance from last known position
		DistanceFromLastKnownPosition = (LastKnownPosition - RootComponent->GetComponentLocation()).Size();
		Blackboard->SetValueAsFloat(DistanceFromLastKnownPositionName, DistanceFromLastKnownPosition);

		// If is not searching and is to reach last position, check if able to reach it
		if (!bIsSearching && !bCannotReachLastKnownPosition)
		{
			CheckIfAbleToReachLastKnownPosition(DeltaSeconds);
			return;
		}

		SearchTick(DeltaSeconds);
		break;
	case EState::Patrolling:
		UpdateCurrentPatrolNode();
		break;
	default:
		break;
	}
}


/*
*	Method that resets the state's values.
*/
void APatrollingAIController::EndState(EState State)
{
	switch (State)
	{
	case EState::Patrolling:
		CurrentPatrolNode = NULL;
		Blackboard->SetValueAsObject(CurrentPatrolNodeName, CurrentPatrolNode);

		CurrentPatrolNodeIndex = -1;
		break;
	case EState::Searching:
		LastKnownPosition = FVector::ZeroVector;
		Blackboard->SetValueAsVector(LastKnownPositionName, LastKnownPosition);

		DistanceFromLastKnownPosition = FLT_MAX;
		Blackboard->SetValueAsFloat(DistanceFromLastKnownPositionName, DistanceFromLastKnownPosition);

		// Reinitialize variables
		bIsSearching = false;
		bCannotReachLastKnownPosition = false;
		Blackboard->SetValueAsFloat(bCannotReachLastKnownPositionName, bCannotReachLastKnownPosition);
		break;
	case EState::Attacking:
		DistanceFromPlayerTarget = FLT_MAX;
		Blackboard->SetValueAsFloat(DistanceFromPlayerTargetName, DistanceFromPlayerTarget);
		break;
	default:
		break;
	}
}

/*
 *	If we are in range of the current patrol node, set the current node to the next 
 *	patrol node.
 */
void APatrollingAIController::UpdateCurrentPatrolNode()
{
	if (!CurrentPatrolNode || CurrentPatrolNodeIndex < 0) return;

	float distance = (RootComponent->GetComponentLocation() - CurrentPatrolNode->GetActorLocation()).Size();
	if (distance <= RangeFromNode)
	{
		CurrentPatrolNodeIndex = (CurrentPatrolNodeIndex + 1) % PatrollingNodes.Num();
		CurrentPatrolNode = PatrollingNodes[CurrentPatrolNodeIndex];
		Blackboard->SetValueAsObject(CurrentPatrolNodeName, CurrentPatrolNode);
	}
}

/*
 *	Gets the index of the nearest node.
 */
int APatrollingAIController::GetIndexOfNearestNode()
{
	if (PatrollingNodes.Num() == 0) return -1;

	float MinDistance = FLT_MAX;
	int IndexOfMinNode = 0;

	for (int i = 0; i < PatrollingNodes.Num(); i++)
	{
		float distance = (RootComponent->GetComponentLocation() - PatrollingNodes[i]->GetActorLocation()).Size();

		if (distance > MinDistance) continue;

		MinDistance = distance;
		IndexOfMinNode = i;
	}

	return IndexOfMinNode;
}

/*
 *	Method that calculates the rotation of the character for the search.
 *	Also determines if the search is finished or not.
 */
void APatrollingAIController::SearchTick(float DeltaTime)
{
	FRotator rotation = GetPawn()->GetRootComponent()->GetComponentRotation();

	// Set character rotation
	rotation.Yaw += DeltaTime * SearchRotationSpeed;
	GetPawn()->GetRootComponent()->SetWorldRotation(rotation);

	TotalSearchTime += DeltaTime;

	// If not reached end of the search, stop
	if (TotalSearchTime < SearchDuration) return;

	// Reset player target
	CurrentPlayerTarget = NULL;
	Blackboard->SetValueAsObject(CurrentPlayerTargetName, CurrentPlayerTarget);

	// Set finished and end state
	bIsSearchFinished = true;
}

/*
 *	Check if the AI is able to reach the last known position of the target.
 */
void APatrollingAIController::CheckIfAbleToReachLastKnownPosition(float DeltaTime)
{
	FVector currentPosition = GetPawn()->GetRootComponent()->GetComponentLocation();

	float distance = (currentPosition - AILastSearchPosition).Size();

	/* 
	 *	If distance is bigger than the tolerance, reset the search idle time and set the last position.
	 *	return after.
	 */
	if (distance > AIPositionDistanceTolerance)
	{
		TotalSearchIdleTime = 0.f;
		AILastSearchPosition = currentPosition;
		return;
	}

	TotalSearchIdleTime += DeltaTime;

	// If total idle time is lower than the max duration, stop
	if (TotalSearchIdleTime <= SearchIdleMaxDuration) return;

	// Reset idle time and set Cannot reach bool
	TotalSearchIdleTime = 0.f;
	bCannotReachLastKnownPosition = true;
	Blackboard->SetValueAsFloat(bCannotReachLastKnownPositionName, bCannotReachLastKnownPosition);
}

/*
 *	Updates the current target. Checks if it is still visible, or if there is a better suited 
 *	target to replace the current one.
 */
void APatrollingAIController::UpdatePlayerTarget()
{
	// If current Target is not visible, set to null
	AActor* BestTarget = (IsActorVisible<ABaseCharacter>(CurrentPlayerTarget)) ? CurrentPlayerTarget : NULL;

	// If is rubberman, we do not need to analyze the other characters
	if (!dynamic_cast<ARubberManCharacter*>(BestTarget))
	{
		// Iterate through actors in the perception mesh
		for (int i = 0; i < ActorsInRange.Num(); i++)
		{
			AActor* TempActor = ActorsInRange[i];

			// If is the current target of is not visible, go to next actor
			if (TempActor == BestTarget
				|| !IsActorVisible<AActor>(TempActor)) continue;

			// If is rubberman, stop searching
			if (dynamic_cast<ARubberManCharacter*>(TempActor))
			{
				BestTarget = TempActor;
				break;
			}
			BestTarget = TempActor;
		}

		// If is null, set to the current player target
		BestTarget = (!BestTarget) ? CurrentPlayerTarget : BestTarget;
	}
	
	// If best target is not the current one, change it
	if (BestTarget != CurrentPlayerTarget)
	{
		CurrentPlayerTarget = BestTarget;
		Blackboard->SetValueAsObject(CurrentPlayerTargetName, CurrentPlayerTarget);
	}

	// Test target visibility
	bool TempVisibility = IsActorVisible<ABaseCharacter>(CurrentPlayerTarget);
	TempVisibility = (TempVisibility && CurrentState == EState::Searching) 
		? ActorsInRange.Contains(CurrentPlayerTarget) : TempVisibility;

	// If visibility has changed, update it
	if (TempVisibility != bPlayerTargetVisible)
	{
		bPlayerTargetVisible = TempVisibility;
		Blackboard->SetValueAsBool(bPlayerTargetVisibleName, bPlayerTargetVisible);
	}
}