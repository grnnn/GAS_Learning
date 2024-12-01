// Copyright Taylor Owen-Milner


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraEffectActor::ApplyEffectToActor(AActor* Actor, TSubclassOf<UGameplayEffect> EffectClass, EEffectRemovalPolicy RemovalPolicy)
{
	auto AbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (!AbilitySystem)
		return;

	check(EffectClass);
	auto EffectContextHandle = AbilitySystem->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	auto EffectSpecHandle = AbilitySystem->MakeOutgoingSpec(EffectClass, ActorLevel, EffectContextHandle);
	auto ActiveEffectHandle = AbilitySystem->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	auto DurationPolicy = EffectSpecHandle.Data->Def->DurationPolicy;
	if (DurationPolicy == EGameplayEffectDurationType::Infinite && RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveOnEndOverlapEffects.Add(ActiveEffectHandle);
	}
}

void AAuraEffectActor::OnOverlap(AActor* Actor)
{
	bool DestroyAfterApplyingEffects = false;
	for (auto EffectApplication : GameplayEffects)
	{
		if (EffectApplication.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToActor(Actor, EffectApplication.GameplayEffect, EffectApplication.EffectRemovalPolicy);
			DestroyAfterApplyingEffects = EffectApplication.bDestroyOnEffectApplication;
		}
	}

	if (DestroyAfterApplyingEffects)
	{
		Destroy();
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* Actor)
{
	bool DestroyAfterApplyingEffects = false;
	bool DestroyAfterRemovingEffects = false;
	for (auto EffectApplication : GameplayEffects)
	{
		if (EffectApplication.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			check(EffectApplication.EffectRemovalPolicy == EEffectRemovalPolicy::DoNotRemove);
			ApplyEffectToActor(Actor, EffectApplication.GameplayEffect, EffectApplication.EffectRemovalPolicy);
			DestroyAfterApplyingEffects = EffectApplication.bDestroyOnEffectApplication;
		}
		else if (EffectApplication.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			for (auto ActiveEffectHandle : ActiveOnEndOverlapEffects)
			{
				auto AbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
				if (!AbilitySystem)
					return;

				AbilitySystem->RemoveActiveGameplayEffect(ActiveEffectHandle, 1);
			}
			ActiveOnEndOverlapEffects.Empty();
			DestroyAfterRemovingEffects = EffectApplication.bDestroyOnEffectRemoval;
		}
	}

	if (DestroyAfterApplyingEffects || DestroyAfterRemovingEffects)
	{
		Destroy();
	}
}

