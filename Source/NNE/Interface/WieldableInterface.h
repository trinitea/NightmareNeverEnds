#pragma once

#include "WieldableInterface.generated.h"

/** Class needed to support InterfaceCast<IToStringInterface>(Object) */
UINTERFACE(MinimalAPI)
class UWieldable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IWieldable
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual void UseMain();
	virtual void UseSecondary();
	virtual void Discard();
};