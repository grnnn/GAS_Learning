// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

struct FGameplayTag;
class UAnimMontage;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetCombatLevel();
	virtual FVector GetCombatSocketLocation();
	virtual void Die() = 0;
	virtual TArray<FGameplayTag> GetResistances();
	
	UFUNCTION(BlueprintCallable)
	virtual void SetFacingLocation(const FVector& Location);

	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetHitReactMontage();
};
