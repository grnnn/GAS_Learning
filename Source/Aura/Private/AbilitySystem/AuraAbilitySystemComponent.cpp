// Copyright Taylor Owen-Milner


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* Asc, const FGameplayEffectSpec& EffectSpec,
                                                FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer Tags;
	EffectSpec.GetAllAssetTags(Tags);
	EffectAssetTags.Broadcast(Tags);
}
