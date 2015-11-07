// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StartAttacking.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UBTTask_StartAttacking : public UBTTaskNode
{
	GENERATED_BODY()
	
	// CONSTRUCTOR
public:
	UBTTask_StartAttacking(const FObjectInitializer& ObjectInitializer);

	// METHODS
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory) override;	
};
