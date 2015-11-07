// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StartPatrolling.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UBTTask_StartPatrolling : public UBTTaskNode
{
	GENERATED_BODY()
	
	// CONSTRUCTOR
public:
	UBTTask_StartPatrolling(const FObjectInitializer& ObjectInitializer);
	
	// Methods
public:
	virtual EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory) override;
};
