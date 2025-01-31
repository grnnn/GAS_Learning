// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GecDamage.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UGecDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGecDamage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	static float GetAttributeValue(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                              const FAggregatorEvaluateParameters& EvalParams, const FGameplayEffectAttributeCaptureDefinition& CaptureDef);
};
