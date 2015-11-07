// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "CustomActionBindingComponent.h"

/*
 *	Constructor for a single binding.
 */
UCustomActionBindingComponent::UCustomActionBindingComponent(FCustomBindingData Binding, bool bIsBindedByDefault, UInputComponent* InputComponent)
{
	Bindings.Add(Binding);
	bIsBinded = true;
	this->bIsBindedByDefault = bIsBindedByDefault;

	// If is not binded by default, then unbind now
	if (!bIsBindedByDefault && InputComponent)
	{
		UnbindAction(InputComponent);
	}
}

/*
 *	Constructor for an array of bindings.
 */
UCustomActionBindingComponent::UCustomActionBindingComponent(TArray<FCustomBindingData> Bindings, bool bIsBindedByDefault, UInputComponent* InputComponent)
{
	this->Bindings = Bindings;
	bIsBinded = true;
	this->bIsBindedByDefault = bIsBindedByDefault;

	// If is not binded by default, then unbind now
	if (!bIsBindedByDefault && InputComponent)
	{
		UnbindAction(InputComponent);
	}
}


/*
 *	Method that binds the action to the input component.
 */
bool UCustomActionBindingComponent::BindAction(UInputComponent* InputComponent)
{
	// Return false since it is locked
	if (bIsLocked) return false;

	// Return true since it is binded
	if (bIsBinded) return true;

	for (int i = 0; i < Bindings.Num(); i++)
	{
		if (Bindings[i].IsAxis)
		{
			InputComponent->AxisBindings.Add(Bindings[i].BindedAxisMapping);
		}
		else
		{
			FInputActionBinding binding = InputComponent->AddActionBinding(Bindings[i].BindedActionMapping);
			Bindings[i].BindedActionMapping = binding;
		}
	}

	bIsBinded = true;
	return true;
}

/*
 *	Method that unbinds the action from the input component.
 */
bool UCustomActionBindingComponent::UnbindAction(UInputComponent* InputComponent)
{
	// Return false since it is locked
	if (bIsLocked) return false;

	// Return true since it is already unbinded
	if (!bIsBinded) return true;

	for (int i = 0; i < Bindings.Num(); i++)
	{
		if (Bindings[i].IsAxis)
		{
			// Find index of the binding
			int max = InputComponent->AxisBindings.Num();
			int index = -1;
			for (int j = 0; j < max; j++)
			{
				FInputAxisBinding binding = InputComponent->AxisBindings[j];

				if (CompareAxisStruct(&binding, &Bindings[i].BindedAxisMapping))
				{
					index = j;
					break;
				}
			}

			// Remove if valid
			if (index >= 0)
			{
				InputComponent->AxisBindings.RemoveAt(index);
			}
		}
		else
		{
			//Find index of binding
			int max = InputComponent->GetNumActionBindings();
			int index = -1;
			for (int j = 0; j < max; j++)
			{
				FInputActionBinding binding = InputComponent->GetActionBinding(j);
				
				if (CompareActionStruct(&binding, &Bindings[i].BindedActionMapping))
				{
					index = j;
					break;
				}
			}

			// Remove if valid
			if (index >= 0)
			{
				InputComponent->RemoveActionBinding(index);
			}
		}
	}
	bIsBinded = false;
	return true;
}


/*
 *	Checks if the two structs are equal.
 */
bool UCustomActionBindingComponent::CompareActionStruct(FInputActionBinding* a, FInputActionBinding* b)
{
	return (a->ActionDelegate == b->ActionDelegate
		&& a->ActionName == b->ActionName
		&& a->KeyEvent == b->KeyEvent);
}

/*
 *	Check if the two structs are equal.
 */
bool UCustomActionBindingComponent::CompareAxisStruct(FInputAxisBinding* a, FInputAxisBinding* b)
{
	return (a->AxisDelegate == b->AxisDelegate
		&& a->AxisName == b->AxisName
		&& a->AxisValue == b->AxisValue);
}