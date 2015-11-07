// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "PatrollingAIController.h"
#include "BTTask_StartSearching.h"


/*
*	Default Constructor.
*/
UBTTask_StartSearching::UBTTask_StartSearching(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Start Searching";
}


/*
*	Execute Task Method
*/
EBTNodeResult::Type UBTTask_StartSearching::ExecuteTask(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp
		|| !dynamic_cast<APatrollingAIController*>(OwnerComp->GetAIOwner())) return EBTNodeResult::Failed;

	APatrollingAIController* MyController = Cast<APatrollingAIController>(OwnerComp->GetOwner());

	MyController->StartSearchingMode();

	return EBTNodeResult::Succeeded;
}


