// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AttachableActor.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UAttachableActor : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IAttachableActor
{
	GENERATED_IINTERFACE_BODY()

public:

	virtual void HookMe();
	virtual void DropMe();
	virtual void UseMe();
};