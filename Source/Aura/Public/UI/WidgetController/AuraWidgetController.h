// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams()
		: PlayerController(nullptr)
		, PlayerState(nullptr)
		, AbilitySystemComponent(nullptr)
		, AttributeSet(nullptr)
	{
	}

	FWidgetControllerParams(APlayerController* InPlayerController, APlayerState* InPlayerState,
							UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
		: PlayerController(InPlayerController)
		, PlayerState(InPlayerState)
		, AbilitySystemComponent(InAbilitySystemComponent)
		, AttributeSet(InAttributeSet)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& InWidgetControllerParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() {}
	
	virtual void BindCallbacksToDependencies() {}

protected:
	UPROPERTY(BlueprintReadOnly, Category="Widget Controller")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="Widget Controller")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="Widget Controller")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="Widget Controller")
	TObjectPtr<UAttributeSet> AttributeSet;
};
