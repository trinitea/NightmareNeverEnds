// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()

	// CONSTRUCTOR
public:
	UBTTask_Attack(const FObjectInitializer& ObjectInitializer);

	// METHODS
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory) override;	
	virtual void TickTask(class UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
