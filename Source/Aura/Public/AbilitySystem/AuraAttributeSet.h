// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

/**
 * NOTE: This macro will safely implement the glue code needed to declare and replicate a gameplay attribute.
 *	Unfortunately, Unreal Engine will not recognize the attribute as a UPROPERTY if you wrap it in a macro like this.
 *	Make sure to expand this macro in the header file where you declare the attribute.
 */
#define GAMEPLAY_ATTRIBUTE_DECL(AttrName, CategoryName) \
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_##AttrName, Category = CategoryName) \
	FGameplayAttributeData AttrName; \
	UFUNCTION() \
	void OnRep_##AttrName(const FGameplayAttributeData& Old##AttrName);

// This macro is fine, as far as I can tell
#define GAMEPLAY_ATTRIBUTE_DEF(ClassName, AttrName) \
	void ClassName##::OnRep_##AttrName(const FGameplayAttributeData& Old##AttrName) \
	{ \
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, AttrName, Old##AttrName); \
	}

#define GAMEPLAY_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAuraAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// GAMEPLAY_ATTRIBUTE_DECL(Health, "Vital Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes") FGameplayAttributeData Health; UFUNCTION() void OnRep_Health(const FGameplayAttributeData& OldHealth);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)
	
	// GAMEPLAY_ATTRIBUTE_DECL(MaxHealth, "Vital Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes") FGameplayAttributeData MaxHealth; UFUNCTION() void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)
	
	// GAMEPLAY_ATTRIBUTE_DECL(Mana, "Vital Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes") FGameplayAttributeData Mana; UFUNCTION() void OnRep_Mana(const FGameplayAttributeData& OldMana);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)
	
	// GAMEPLAY_ATTRIBUTE_DECL(MaxMana, "Vital Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes") FGameplayAttributeData MaxMana; UFUNCTION() void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)
};
