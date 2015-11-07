// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "PatrollingAIController.h"
#include "BTTask_Search.h"


/*
*	Default Constructor.
*/
UBTTask_Search::UBTTask_Search(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Search";
}


/*
*	Execute Task Method
*/
EBTNodeResult::Type UBTTask_Search::ExecuteTask(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp
		|| !dynamic_cast<APatrollingAIController*>(OwnerComp->GetAIOwner())) return EBTNodeResult::Failed;
	
	APatrollingAIController* MyController = Cast<APatrollingAIController>(OwnerComp->GetOwner());
	
	// If search is finished, return succeeded, else in progress
	return (MyController->Search()) ? EBTNodeResult::Succeeded : EBTNodeResult::InProgress;
}



