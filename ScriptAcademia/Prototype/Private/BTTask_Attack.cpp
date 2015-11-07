// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "PatrollingAIController.h"
#include "BTTask_Attack.h"


/*
*	Default Constructor.
*/
UBTTask_Attack::UBTTask_Attack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Attack";
	bNotifyTick = true;
}


/*
*	Execute Task Method
*/
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp
		|| !dynamic_cast<APatrollingAIController*>(OwnerComp->GetAIOwner())) return EBTNodeResult::Failed;

	APatrollingAIController* MyController = Cast<APatrollingAIController>(OwnerComp->GetOwner());
	
	// Return succeeded if attack finished or inprogress if it is not
	return (MyController->Attack()) ? EBTNodeResult::Succeeded : EBTNodeResult::InProgress;
}


/*
 *	Tick Task, overriden since we need to execute the task more than once.
 */
void UBTTask_Attack::TickTask(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!OwnerComp
		|| !dynamic_cast<APatrollingAIController*>(OwnerComp->GetAIOwner())) return;

	APatrollingAIController* MyController = Cast<APatrollingAIController>(OwnerComp->GetOwner());

	MyController->Attack();
}