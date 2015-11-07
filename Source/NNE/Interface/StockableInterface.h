#pragma once

#include "StockableInterface.generated.h"

/** Class needed to support InterfaceCast<IToStringInterface>(Object) */
UINTERFACE(MinimalAPI)
class UStockable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IStockable
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual void Take();
	virtual void Drop();
};