// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"

#include "Interaction/CombatInterface.h"

#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void ApplyDamageToReceiver(AActor* DamageReceiver);

	UFUNCTION(BlueprintCallable)
	FTaggedMontage GetAttackMontage(FGameplayTag MontageTag, const TArray<FTaggedMontage>& TaggedMontages) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageModifiers;
};
