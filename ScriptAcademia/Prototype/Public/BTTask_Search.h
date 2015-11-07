// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Search.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UBTTask_Search : public UBTTaskNode
{
	GENERATED_BODY()
	
	// CONSTRUCTOR
public:
	UBTTask_Search(const FObjectInitializer& ObjectInitializer);

	// METHODS
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory) override;
	
};
