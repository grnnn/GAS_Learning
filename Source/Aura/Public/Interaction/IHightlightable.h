// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHightlightable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIHightlightable : public UInterface
{
	GENERATED_BODY()
};

class AURA_API IIHightlightable
{
	GENERATED_BODY()

public:
	virtual void Highlight() = 0;
	virtual void UnHighlight() = 0;
};
