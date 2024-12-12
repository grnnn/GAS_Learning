// Copyright Taylor Owen-Milner


#include "AbilitySystem/MMC/MmcMaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"

#include "Interaction/CombatInterface.h"

UMmcMaxHealth::UMmcMaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMmcMaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max(0.f, Vigor);

	auto CombatObject = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	int32 Level = CombatObject->GetCombatLevel();
	
	return 10.f + ((Vigor - 1.f) * 1.f) + ((Level - 1.f) * 1.f);
}
