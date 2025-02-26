// Copyright Taylor Owen-Milner


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(UObject* WorldContextObject)
{
	if (auto Pc = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (auto Hud = Cast<AAuraHUD>(Pc->GetHUD()))
		{
			auto Ps = Pc->GetPlayerState<AAuraPlayerState>();
			auto AttributeSet = Ps->GetAttributeSet();
			auto Asc = Ps->GetAbilitySystemComponent();
			auto CombatInterface = Cast<ICombatInterface>(Pc->GetPawn());
			return Hud->CreateOverlayWcOnce({Pc, Ps, Asc, AttributeSet, CombatInterface});
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(UObject* WorldContextObject)
{
	if (auto Pc = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (auto Hud = Cast<AAuraHUD>(Pc->GetHUD()))
		{
			auto Ps = Pc->GetPlayerState<AAuraPlayerState>();
			auto AttributeSet = Ps->GetAttributeSet();
			auto Asc = Ps->GetAbilitySystemComponent();
			auto CombatInterface = Cast<ICombatInterface>(Pc->GetPawn());
			return Hud->CreateAttributeMenuWcOnce({Pc, Ps, Asc, AttributeSet, CombatInterface});
		}
	}

	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(UObject* WorldContextObject, UAbilitySystemComponent* AscInput,
	ECharacterClass Class, float Level)
{
	if (auto AuraGm = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		auto AActor = AscInput->GetAvatarActor();
		
		// primary attributes
		auto ClassDefaults = AuraGm->CharacterClassInfo->GetClassDefaults(Class);
		auto EffectContext = AscInput->MakeEffectContext();
		EffectContext.AddSourceObject(AActor);
		auto SpecHandle = AscInput->MakeOutgoingSpec(ClassDefaults.PrimaryAttributes, Level, EffectContext);
		AscInput->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		// secondary attributes
		EffectContext = AscInput->MakeEffectContext();
		EffectContext.AddSourceObject(AActor);
		SpecHandle = AscInput->MakeOutgoingSpec(AuraGm->CharacterClassInfo->SecondaryAttributes, Level, EffectContext);
		AscInput->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		// vital attributes
		EffectContext = AscInput->MakeEffectContext();
		EffectContext.AddSourceObject(AActor);
		SpecHandle = AscInput->MakeOutgoingSpec(AuraGm->CharacterClassInfo->VitalAttributes, Level, EffectContext);
		AscInput->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		// resistance attributes
		EffectContext = AscInput->MakeEffectContext();
		EffectContext.AddSourceObject(AActor);
		SpecHandle = AscInput->MakeOutgoingSpec(AuraGm->CharacterClassInfo->ResistanceAttributes, Level, EffectContext);
		AscInput->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(UObject* WorldContextObject, UAbilitySystemComponent* AscInOut, ECharacterClass Class, float Level)
{
	auto AuraGm = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (not AuraGm)
		return;

	auto SharedAbilities = AuraGm->CharacterClassInfo->SharedAbilities;
	for (auto AbilityClass : SharedAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityClass, Level);
		AscInOut->GiveAbility(AbilitySpec);
	}

	auto ClassAbilities = AuraGm->CharacterClassInfo->GetClassDefaults(Class).ClassAbilities;
	for (auto AbilityClass : ClassAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityClass, Level);
		AscInOut->GiveAbility(AbilitySpec);
	}
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (auto Context = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return Context->IsCriticalHit();
	}

	return false;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (auto Context = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return Context->IsBlockedHit();
	}

	return false;
}

bool UAuraAbilitySystemLibrary::IsResistedHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (auto Context = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return Context->IsResistedHit();
	}

	return false;
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& ContextHandle, bool bValue)
{
	if (auto Context = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		Context->SetIsCriticalHit(bValue);
	}
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& ContextHandle, bool bValue)
{
	if (auto Context = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		Context->SetIsBlockedHit(bValue);
	}
}

void UAuraAbilitySystemLibrary::SetIsResistedHit(FGameplayEffectContextHandle& ContextHandle, bool bValue)
{
	if (auto Context = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		Context->SetIsResistedHit(bValue);
	}
}

void UAuraAbilitySystemLibrary::GetLiveActorsWithinRadius(UObject* WorldContextObject, const FVector& Location,
	float Radius, const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutActors)
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> OverlapResults;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(OverlapResults, Location, FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius), QueryParams);
	}

	for (auto Overlap : OverlapResults)
	{
		auto Actor = Overlap.GetActor();
		if (Actor && Actor->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Actor))
		{
			OutActors.AddUnique(Actor);
		}
	}
}

bool UAuraAbilitySystemLibrary::AreActorsFriends(AActor* ActorA, AActor* ActorB)
{
	check(ActorA && ActorB);

	static TArray<FName> FriendTags = {"Player", "Enemy"};
	for (auto Tag : FriendTags)
	{
		if (ActorA->ActorHasTag(Tag) && ActorB->ActorHasTag(Tag))
		{
			return true;
		}
	}

	return false;
}
