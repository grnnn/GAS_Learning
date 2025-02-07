// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FComponentCleanedUpDelegate);

UENUM(BlueprintType)
enum class EDamageState : uint8
{
	Normal,
	Critical,
	CriticalResisted,
	Blocked,
	BlockedResisted,
	Resisted,
};

/**
 * 
 */
UCLASS()
class AURA_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, EDamageState DamageState);

	UFUNCTION(BlueprintNativeEvent)
	bool IsDamageTextVisible();

	UPROPERTY(BlueprintReadWrite)
	FComponentCleanedUpDelegate OnComponentCleanedUp;
};
