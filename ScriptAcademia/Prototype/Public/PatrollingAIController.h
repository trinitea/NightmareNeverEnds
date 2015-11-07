// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "PatrollingAIController.generated.h"

#define COLLISION_TRACE ECC_PhysicsBody

UENUM(BlueprintType)
enum class EState : uint8
{
	None UMETA(DisplayName = "None"),
	Patrolling UMETA(DisplayName = "Patrolling"),
	Searching UMETA(DisplayName = "Searching"),
	Attacking UMETA(DisplayName = "Attacking")
};


/**
 *	Class for a patrolling AI.
 */
UCLASS()
class PROTOTYPE_API APatrollingAIController : public AAIController
{
	GENERATED_BODY()
			
	//METHODS
public:
	// Constructor
	APatrollingAIController(const FObjectInitializer& ObjectInitializer);

	// METHODS
public:
	void StartPatrollingMode();
	void StartAttackingMode();
	void StartSearchingMode();
	bool Attack();
	bool Search();
	void OnOverlapBegin_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnOverlapEnd_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Overriden methods
private:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);

	// METHODS
private:
	int GetIndexOfNearestNode();
	void UpdateCurrentPatrolNode();
	void EndState(EState State);
	void SearchTick(float DeltaTime);
	void CheckIfAbleToReachLastKnownPosition(float DeltaTime);
	void UpdatePlayerTarget();
	
	// TEMPLATE FUNCTIONS
private:

	/*
	 *	Raytraces from this actor to the actor given in parameter. If actor is 
	 *	directly hit (no actor in between) return true.
	 */
	template <class ActorType>
	bool IsActorVisible(AActor* Actor)
	{
		if (!Actor
			|| !Actor->GetRootComponent()) return false;
		
		FHitResult Hit;

		// Ignore collision with pawn
		FCollisionQueryParams Line(FName("Collision param"), false, GetPawn());

		// Commented this, but it is useful if we want to see the ray traces
		//const FName TraceTag("MyTraceTag");
		//GetWorld()->DebugDrawTraceTag = TraceTag;
		//Line.TraceTag = TraceTag;

		// Setup trace vectors
		FVector OurPosition = RootComponent->GetComponentLocation();
		FVector Distance = Actor->GetRootComponent()->GetComponentLocation() - OurPosition;
		FVector EndTrace = OurPosition + ((Distance/Distance.Size()) * (Distance.Size() + 100));
		
		GetWorld()->LineTraceSingle(Hit, OurPosition, EndTrace, COLLISION_TRACE, Line);

		// Return if hit actor is of the type required
		return !(!Hit.GetActor() || !dynamic_cast<ActorType*>(Hit.GetActor()));
	}
	
	// EXPOSED VARIABLES
public:
	// Blackboard to use
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Behavior)
	class UBlackboardData* BlackboardData = NULL;

	// Behavior tree used
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Behavior)
	class UBehaviorTree* BehaviorTree = NULL;
	
	// Range from patrol node
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PatrolOptions)
		float RangeFromNode = 200;

	// SEARCH OPTIONS
public:
	// Search target rotation speed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SearchOptions)
		float SearchRotationSpeed = 20.f;
	
	// Search target duration
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SearchOptions)
		float SearchDuration = 5.f;

	// Time AI can stay idle in search mode before exit condition
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SearchOptions)
		float SearchIdleMaxDuration = 3.f;
	
	// Distance tolerance to determine if AI has moved
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SearchOptions)
		float AIPositionDistanceTolerance = 2.f;

	// BLACKBOARD NAMES
public:
	//Names of variables in the blackboard
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BlackboardNames)
		FName CurrentPatrolNodeName = FName(TEXT("CurrentPatrolNode"));
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BlackboardNames)
		FName CurrentPlayerTargetName = FName(TEXT("CurrentPlayerTarget"));
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BlackboardNames)
		FName bPlayerTargetVisibleName = FName(TEXT("bPlayerTargetVisible"));
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BlackboardNames)
		FName LastKnownPositionName = FName(TEXT("LastKnownPosition"));
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BlackboardNames)
		FName DistanceFromPlayerTargetName = FName(TEXT("DistanceFromPlayerTarget"));
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BlackboardNames)
		FName DistanceFromLastKnownPositionName = FName(TEXT("DistanceFromLastKnownPosition"));
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BlackboardNames)
		FName CurrentStateName = FName(TEXT("CurrentState"));
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BlackboardNames)
		FName PatrollingNodesName = FName(TEXT("PatrollingNodes"));
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BlackboardNames)
		FName bCannotReachLastKnownPositionName = FName(TEXT("bCannotReachLastKnownPosition"));
	
	
	//PRIVATE VARIABLES
private:
	// Blackboard
	class UBlackboardComponent* Blackboard = NULL;

	// Patrolling Nodes
	TArray<AActor*> PatrollingNodes;

	// Actors currently in the perception trigger
	TArray<AActor*> ActorsInRange;

	// Current patrol node
	AActor* CurrentPatrolNode = NULL;

	// Player target
	AActor* CurrentPlayerTarget = NULL;

	EState CurrentState = EState::None;
	
	// Last known position of the target
	FVector LastKnownPosition;

	// Keep track of the AI position to make sure it is not stuck while searching
	FVector AILastSearchPosition;

	// Search timers
	float TotalSearchTime = 0.f;
	float TotalSearchIdleTime = 0.f;
	
	float DistanceFromPlayerTarget = FLT_MAX;
	float DistanceFromLastKnownPosition = FLT_MAX;

	int CurrentPatrolNodeIndex = 0;

	// If target visible
	bool bPlayerTargetVisible = false;

	// Change to true if AI not able to reach position
	bool bCannotReachLastKnownPosition = false;

	// If AI is currently searching
	bool bIsSearching = false;

	// If AI has finished it's search
	bool bIsSearchFinished = false;
};
