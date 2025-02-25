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
