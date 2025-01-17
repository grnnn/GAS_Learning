// Copyright Taylor Owen-Milner


#include "UI/WidgetController/EnemyWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UEnemyWidgetController::BroadcastInitialValues()
{
	auto Attributes = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(Attributes->GetHealth());
	OnMaxHealthChanged.Broadcast(Attributes->GetMaxHealth());
}

void UEnemyWidgetController::BindCallbacksToDependencies()
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
}
