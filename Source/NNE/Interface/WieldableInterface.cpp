
#include "NNE.h"
#include "WieldableInterface.h"

UWieldable::UWieldable(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

//This is required for compiling, would also let you know if somehow you called
//the base event/function rather than the over-rided version
void IWieldable::UseMain() { }
void IWieldable::UseSecondary() { }
void IWieldable::Discard() { }