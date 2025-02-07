// Copyright Taylor Owen-Milner


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InWidgetControllerParams)
{
	PlayerController = InWidgetControllerParams.PlayerController;
	PlayerState = InWidgetControllerParams.PlayerState;
	AbilitySystemComponent = InWidgetControllerParams.AbilitySystemComponent;
	AttributeSet = InWidgetControllerParams.AttributeSet;
	PlayerCombatInterface = InWidgetControllerParams.PlayerCombatInterface;
}
