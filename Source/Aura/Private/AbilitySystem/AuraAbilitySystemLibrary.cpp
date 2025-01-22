// Copyright Taylor Owen-Milner


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"

#include "Game/AuraGameModeBase.h"

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
			return Hud->CreateOverlayWcOnce({Pc, Ps, Asc, AttributeSet});
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
			return Hud->CreateAttributeMenuWcOnce({Pc, Ps, Asc, AttributeSet});
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
	}
	
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(UObject* WorldContextObject, UAbilitySystemComponent* AscInOut)
{
	auto AuraGm = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (not AuraGm)
		return;

	auto SharedAbilities = AuraGm->CharacterClassInfo->SharedAbilities;
	for (auto AbilityClass : SharedAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		AscInOut->GiveAbility(AbilitySpec);
	}
}
