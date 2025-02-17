// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraPlayableCharacter.generated.h"

class APlayerState;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayableCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuraPlayableCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// ICombatInterface
	virtual int32 GetCombatLevel() override;
	UFUNCTION(BlueprintCallable)
	virtual void SetFacingLocation(const FVector& Location) override;

protected:
	virtual void InitAbilityActorInfo() override;
	
private:
	void SetAbilitySystemPointers();
	void InitOverlay();
};
