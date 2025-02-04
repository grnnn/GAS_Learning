// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library|Widget Controller")
	static UOverlayWidgetController* GetOverlayWidgetController(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library|Widget Controller")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library|Character Class Defaults")
	static void InitializeDefaultAttributes(UObject* WorldContextObject, UAbilitySystemComponent* AscInput, ECharacterClass Class, float Level);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library|Character Class Defaults")
	static void GiveStartupAbilities(UObject* WorldContextObject, UAbilitySystemComponent* AscInOut);

	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library|Gameplay Effects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library|Gameplay Effects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library|Gameplay Effects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bValue);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library|Gameplay Effects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bValue);
};
