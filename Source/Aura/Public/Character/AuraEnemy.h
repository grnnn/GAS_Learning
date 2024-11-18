// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/IHightlightable.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IIHightlightable
{
	GENERATED_BODY()
public:
	AAuraEnemy();
	
	virtual void Highlight() override;
	virtual void UnHighlight() override;

protected:
	virtual void BeginPlay() override;
};
