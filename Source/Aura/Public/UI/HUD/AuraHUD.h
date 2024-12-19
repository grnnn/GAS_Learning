// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAuraUserWidget;
class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	UOverlayWidgetController* CreateOverlayWcOnce(const FWidgetControllerParams& Params);
	UAttributeMenuWidgetController* CreateAttributeMenuWcOnce(const FWidgetControllerParams& Params);

	void InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState,
					 UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet);
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
