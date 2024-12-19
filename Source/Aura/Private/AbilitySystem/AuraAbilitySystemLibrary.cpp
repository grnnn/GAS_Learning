// Copyright Taylor Owen-Milner


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

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
