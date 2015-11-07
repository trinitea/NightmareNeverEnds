
#include "NNE.h"
#include "StockableInterface.h"

UStockable::UStockable(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

//This is required for compiling, would also let you know if somehow you called
//the base event/function rather than the over-rided version
void IStockable::Take() { }
void IStockable::Drop() { }