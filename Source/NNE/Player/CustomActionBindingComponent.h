// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h" // <--- what is that ?
#include "GameFramework/PlayerController.h"
#include "CustomActionBindingComponent.generated.h"


// ENUM FOR LOCKING ACTIONS
UENUM(BlueprintType)
enum class EBindingAction : uint8
{
	None UMETA(DisplayName = "None"),
	Movement UMETA(DisplayName = "Movement"),
	CameraMovement UMETA(DisplayName = "CameraMovement")
};

/*
*	Returns value of the Enum. Used because the arrays cannot
*	take an Enum
*/
FORCEINLINE uint8 GetEnumValue(EBindingAction enumVal)
{
	return (uint8)enumVal;
}

/*
*	Struct for the binding data generated after registering the methods.
*/
USTRUCT()
struct FCustomBindingData
{
	GENERATED_USTRUCT_BODY()

		// CONSTRUCTORS
public:

	FCustomBindingData() {}

	FCustomBindingData(FInputAxisBinding BindedAxisMapping)
	{
		IsAxis = true;
		this->BindedAxisMapping = BindedAxisMapping;
	}

	FCustomBindingData(FInputActionBinding BindedActionMapping)
	{
		IsAxis = false;
		this->BindedActionMapping = BindedActionMapping;
	}


	// VARIABLES
public:
	bool IsAxis;

	FInputAxisBinding BindedAxisMapping;
	FInputActionBinding BindedActionMapping;
};


/*
*	Class that contains the binding data, and that binds
*	and unbinds the methods.
*/
class UCustomActionBindingComponent
{
	// CONSTRUCTORS
public:
	UCustomActionBindingComponent(FCustomBindingData Binding, bool bIsBindedByDefault = true, UInputComponent* InputComponent = NULL);
	UCustomActionBindingComponent(TArray<FCustomBindingData> Bindings, bool bIsBindedByDefault = true, UInputComponent* InputComponent = NULL);

	//BINDING METHODS, RETURNS TRUE IF SUCCEEDED
public:
	bool BindAction(UInputComponent* InputComponent);
	bool UnbindAction(UInputComponent* InputComponent);
	inline void SetLocked(bool bIsLocked)
	{
		this->bIsLocked = bIsLocked;
	}
	inline bool IsLocked()
	{
		return bIsLocked;
	}
	inline bool IsBindedByDefault()
	{
		return bIsBindedByDefault;
	}

	// OTHER METHODS
private:
	//bool CompareActionStruct(FInputActionBinding* a, FInputActionBinding* b);
	//bool CompareAxisStruct(FInputAxisBinding* a, FInputAxisBinding* b);

	// VARIABLES
private:
	bool bIsBinded;
	bool bIsLocked = false;
	bool bIsBindedByDefault;
	TArray<FCustomBindingData> Bindings;
};