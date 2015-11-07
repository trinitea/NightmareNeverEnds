// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "PatrollingAIController.h"
#include "BTTask_StartAttacking.h"

/*
 *	Default Constructor.
 */
UBTTask_StartAttacking::UBTTask_StartAttacking(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Start Attacking";
}

/*
 *	Execute Task Method
 */
EBTNodeResult::Type UBTTask_StartAttacking::ExecuteTask(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp
		|| !dynamic_cast<APatrollingAIController*>(OwnerComp->GetAIOwner())) return EBTNodeResult::Failed;

	APatrollingAIController* MyController = Cast<APatrollingAIController>(OwnerComp->GetOwner());

	MyController->StartAttackingMode();

	return EBTNodeResult::Succeeded;
}

