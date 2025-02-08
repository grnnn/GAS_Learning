// Copyright Taylor Owen-Milner


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	check(PlayerCombatInterface);
	
	auto AuraAs = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair : AuraAs->TagsToAttributeGetters)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value.Execute());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	auto AuraAs = CastChecked<UAuraAttributeSet>(AttributeSet);
	auto ResistancesTag = FAuraGameplayTags::Get().Attributes_Resistance;
	for (auto Pair : AuraAs->TagsToAttributeGetters)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value.Execute()).AddLambda(
			[this, Pair, AuraAs](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value.Execute());
			});
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag,
	const FGameplayAttribute& Attribute)
{
	auto Info = AttributeInfo->GetAttributeInfo(Tag);
	Info.Value = Attribute.GetNumericValue(AttributeSet);
	Info.Tag = Tag;
	AttributeInfoDelegate.Broadcast(Info);
}
