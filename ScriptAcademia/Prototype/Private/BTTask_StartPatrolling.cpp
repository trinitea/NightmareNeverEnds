// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "PatrollingAIController.h"
#include "BTTask_StartPatrolling.h"


/*
*	Default Constructor.
*/
UBTTask_StartPatrolling::UBTTask_StartPatrolling(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Start Patrolling";
}


/*
*	Execute Task Method
*/
EBTNodeResult::Type UBTTask_StartPatrolling::ExecuteTask(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp 
		|| !dynamic_cast<APatrollingAIController*>(OwnerComp->GetAIOwner())) return EBTNodeResult::Failed;

	APatrollingAIController* MyController = Cast<APatrollingAIController>(OwnerComp->GetOwner());

	MyController->StartPatrollingMode();

	return EBTNodeResult::Succeeded;
}