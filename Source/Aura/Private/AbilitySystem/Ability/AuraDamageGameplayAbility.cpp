// Copyright Taylor Owen-Milner


#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::ApplyDamageToReceiver(AActor* DamageReciever)
{
	auto SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (auto DamageModifier : DamageModifiers)
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageModifier.Key, DamageModifier.Value.GetValueAtLevel(GetAbilityLevel()));
	}
	auto TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(DamageReciever);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetAsc);
}

FTaggedMontage UAuraDamageGameplayAbility::GetAttackMontage(FGameplayTag MontageTag, const TArray<FTaggedMontage>& TaggedMontages) const
{
	for (auto Montage : TaggedMontages)
	{
		if (Montage.Tag == MontageTag)
		{
			return Montage;
		}
	}

	int index = FMath::RandRange(0, TaggedMontages.Num() - 1);
	if (TaggedMontages.IsValidIndex(index))
	{
		return TaggedMontages[index];
	}

	return {};
}
