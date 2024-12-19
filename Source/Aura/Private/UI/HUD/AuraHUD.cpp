// Copyright Taylor Owen-Milner


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::CreateOverlayWcOnce(const FWidgetControllerParams& Params)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(Params);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::CreateAttributeMenuWcOnce(const FWidgetControllerParams& Params)
{
	if (not AttributeMenuWidgetController)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(Params);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}

	return AttributeMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState,
                           UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass is not set in BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is not set in BP_AuraHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams Params(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	auto WidgetController = CreateOverlayWcOnce(Params);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}
