// Copyright Taylor Owen-Milner


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	
	auto AuraAs = CastChecked<UAuraAttributeSet>(AttributeSet);

	auto StrengthTag = FAuraGameplayTags::Get().Attributes_Primary_Strength;
	auto Info = AttributeInfo->GetAttributeInfo(StrengthTag);
	Info.Value = AuraAs->GetStrength();
	Info.Tag = StrengthTag;
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	
}
