// Copyright Taylor Owen-Milner


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	auto Attributes = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(Attributes->GetHealth());
	OnMaxHealthChanged.Broadcast(Attributes->GetMaxHealth());
	OnManaChanged.Broadcast(Attributes->GetMana());
	OnMaxManaChanged.Broadcast(Attributes->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	auto Attributes = CastChecked<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute())
		.AddLambda([this](auto AttributeChangeData)
			{
				OnHealthChanged.Broadcast(AttributeChangeData.NewValue);
			});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxHealthAttribute())
		.AddLambda([this](auto AttributeChangeData)
			{
				OnMaxHealthChanged.Broadcast(AttributeChangeData.NewValue);
			});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetManaAttribute())
		.AddLambda([this](auto AttributeChangeData)
			{
				OnManaChanged.Broadcast(AttributeChangeData.NewValue);
			});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxManaAttribute())
		.AddLambda([this](auto AttributeChangeData)
			{
				OnMaxManaChanged.Broadcast(AttributeChangeData.NewValue);	
			});

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](auto Tags)
		{
			auto MessageTag = FGameplayTag::RequestGameplayTag("Message");
			for (const auto& Tag : Tags)
			{
				if (Tag.MatchesTag(MessageTag))
				{
					FUiWidgetRow* Row = GetDataTableRowByTag<FUiWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRow.Broadcast(*Row);
				}
			}
		});
}
