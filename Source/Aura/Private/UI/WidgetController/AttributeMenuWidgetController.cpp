// Copyright Taylor Owen-Milner


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	
	auto AuraAs = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (auto& Pair : AuraAs->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value.Execute());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	auto AuraAs = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (auto Pair : AuraAs->TagsToAttributes)
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
