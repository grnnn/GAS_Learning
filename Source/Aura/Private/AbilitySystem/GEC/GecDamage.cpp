// Copyright Taylor Owen-Milner


#include "AbilitySystem/GEC/GecDamage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilityTypes.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);
	}
};

static const AuraDamageStatics& GetDamageStatics()
{
	static AuraDamageStatics Statics;
	return Statics;
}

UGecDamage::UGecDamage()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalResistanceDef);
}

void UGecDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	auto SourceAsc = ExecutionParams.GetSourceAbilitySystemComponent();
	auto TargetAsc = ExecutionParams.GetTargetAbilitySystemComponent();
	auto SourceActor = SourceAsc ? SourceAsc->GetOwner() : nullptr;
	auto TargetActor = TargetAsc ? TargetAsc->GetOwner() : nullptr;
	auto TargetCombatInterface = Cast<ICombatInterface>(TargetActor);
	auto Spec = ExecutionParams.GetOwningSpec();
	auto Context = static_cast<FAuraGameplayEffectContext*>(Spec.GetContext().Get());

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Damage = 0.f;
	bool bIsResisted = false;
	for (auto TypeToResistance : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		float Resistance = GetResistanceValue(TypeToResistance.Value, TargetCombatInterface, ExecutionParams, EvalParams);
		Damage += (Spec.GetSetByCallerMagnitude(TypeToResistance.Key, false) - Resistance);

		if (Resistance > 0)
		{
			bIsResisted = true;
		}
	}
	
	float TargetArmor = GetAttributeValue(ExecutionParams, EvalParams, GetDamageStatics().ArmorDef);
	float ArmorPenetration = GetAttributeValue(ExecutionParams, EvalParams, GetDamageStatics().ArmorPenetrationDef);
	float TargetBlockChance = GetAttributeValue(ExecutionParams, EvalParams, GetDamageStatics().BlockChanceDef);
	float CriticalHitChance = GetAttributeValue(ExecutionParams, EvalParams, GetDamageStatics().CriticalHitChanceDef);
	float CriticalHitDamage = GetAttributeValue(ExecutionParams, EvalParams, GetDamageStatics().CriticalHitDamageDef);
	float TargetCriticalHitResistance = GetAttributeValue(ExecutionParams, EvalParams, GetDamageStatics().CriticalHitResistanceDef);

	// first apply armor
	Damage -= FMath::Max(0.f,TargetArmor - ArmorPenetration);
	// then apply critical hit chance
	bool bIsCriticalHit = (FMath::Rand() % 100) <= FMath::Max(0.f,CriticalHitChance - TargetCriticalHitResistance);
	bool bIsBlock = (FMath::Rand() % 100) <= TargetBlockChance;
	if (bIsCriticalHit)
	{
		Damage *= 2;
		Damage += CriticalHitDamage;
	}
	// then apply block chance
	else if (bIsBlock)
	{
		Damage /= 2;
	}

	FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	Context->SetIsCriticalHit(bIsCriticalHit);
	Context->SetIsBlockedHit(bIsBlock);
	Context->SetIsResistedHit(bIsResisted);
}

float UGecDamage::GetAttributeValue(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FAggregatorEvaluateParameters& EvalParams, const FGameplayEffectAttributeCaptureDefinition& CaptureDef)
{
	float Value = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvalParams, Value);
	return FMath::Max(0.f, FMath::Floor(Value));
}

float UGecDamage::GetResistanceValue(const FGameplayTag& ResistanceType, ICombatInterface* Target,
	const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvalParams)
{
	if (not Target->GetResistances().Contains(ResistanceType))
	{
		return 0.f;
	}
	
	FGameplayEffectAttributeCaptureDefinition Capture;
	if (ResistanceType == FGameplayTag(FAuraGameplayTags::Get().Attributes_Resistance_Fire))
	{
		Capture = GetDamageStatics().FireResistanceDef;
	}
	else if (ResistanceType == FGameplayTag(FAuraGameplayTags::Get().Attributes_Resistance_Lightning))
	{
		Capture = GetDamageStatics().LightningResistanceDef;
	}
	else if (ResistanceType == FGameplayTag(FAuraGameplayTags::Get().Attributes_Resistance_Arcane))
	{
		Capture = GetDamageStatics().ArcaneResistanceDef;
	}
	else if (ResistanceType == FGameplayTag(FAuraGameplayTags::Get().Attributes_Resistance_Physical))
	{
		Capture = GetDamageStatics().PhysicalResistanceDef;
	}

	if (Capture != FGameplayEffectAttributeCaptureDefinition())
	{
		float Value = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Capture, EvalParams, Value);
		return FMath::Max(0.f, FMath::Floor(Value));
	}

	return 0.f;
}
