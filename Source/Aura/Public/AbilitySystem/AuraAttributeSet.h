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

#define GAMEPLAY_ATTRIBUTE_TAG_MAPPING(AttrName, TagSet) \
	FGetGameplayAttribute Get##AttrName##;	\
	Get##AttrName##.BindStatic(Get##AttrName##Attribute);	\
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_##TagSet##_##AttrName##, Get##AttrName##);

#define GAMEPLAY_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DELEGATE_RetVal(FGameplayAttribute, FGetGameplayAttribute);

USTRUCT()
struct FEffectProperty
{
	GENERATED_BODY()

	UPROPERTY() UAbilitySystemComponent* AbilitySystemComponent = nullptr;
	UPROPERTY() AActor* Actor = nullptr;
	UPROPERTY() AController* Controller = nullptr;
	UPROPERTY() ACharacter* Character = nullptr;
};

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	UPROPERTY() FEffectProperty Source;
	UPROPERTY() FEffectProperty Target;
	UPROPERTY() FGameplayEffectContextHandle EffectContextHandle;
};

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
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	TMap<FGameplayTag, FGetGameplayAttribute> TagsToAttributes;

	/**
	 *	Primary Attributes
	 */

	//GAMEPLAY_ATTRIBUTE_DECL(Strength, "Primary Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes") FGameplayAttributeData Strength; UFUNCTION() void OnRep_Strength(const FGameplayAttributeData& OldStrength);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength)

	//GAMEPLAY_ATTRIBUTE_DECL(Intelligence, "Primary Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes") FGameplayAttributeData Intelligence; UFUNCTION() void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence)

	//GAMEPLAY_ATTRIBUTE_DECL(Dexterity, "Primary Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dexterity, Category = "Primary Attributes") FGameplayAttributeData Dexterity; UFUNCTION() void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Dexterity)

	//GAMEPLAY_ATTRIBUTE_DECL(Vigor, "Primary Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes") FGameplayAttributeData Vigor; UFUNCTION() void OnRep_Vigor(const FGameplayAttributeData& OldVigor);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor)

	/**
	 *	Secondary Attributes
	 */

	//GAMEPLAY_ATTRIBUTE_DECL(Armor, "Secondary Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes") FGameplayAttributeData Armor; UFUNCTION() void OnRep_Armor(const FGameplayAttributeData& OldArmor);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor)

	//GAMEPLAY_ATTRIBUTE_DECL(ArmorPenetration, "Secondary Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes") FGameplayAttributeData ArmorPenetration; UFUNCTION() void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration)

	//GAMEPLAY_ATTRIBUTE_DECL(BlockChance, "Secondary Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes") FGameplayAttributeData BlockChance; UFUNCTION() void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance)

	//GAMEPLAY_ATTRIBUTE_DECL(CriticalHitChance, "Secondary Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes") FGameplayAttributeData CriticalHitChance; UFUNCTION() void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance)

	//GAMEPLAY_ATTRIBUTE_DECL(CriticalHitDamage, "Secondary Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes") FGameplayAttributeData CriticalHitDamage; UFUNCTION() void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage)

	//GAMEPLAY_ATTRIBUTE_DECL(CriticalHitResistance, "Secondary Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes") FGameplayAttributeData CriticalHitResistance; UFUNCTION() void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance)

	//GAMEPLAY_ATTRIBUTE_DECL(HealthRegeneration, "Secondary Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes") FGameplayAttributeData HealthRegeneration; UFUNCTION() void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration)

	//GAMEPLAY_ATTRIBUTE_DECL(ManaRegeneration, "Secondary Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes") FGameplayAttributeData ManaRegeneration; UFUNCTION() void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration)

	// GAMEPLAY_ATTRIBUTE_DECL(MaxHealth, "Vital Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes") FGameplayAttributeData MaxHealth; UFUNCTION() void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)

	// GAMEPLAY_ATTRIBUTE_DECL(MaxMana, "Vital Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes") FGameplayAttributeData MaxMana; UFUNCTION() void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)
	
	/**
	 *	Vital Attributes
	 */
	
	// GAMEPLAY_ATTRIBUTE_DECL(Health, "Vital Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes") FGameplayAttributeData Health; UFUNCTION() void OnRep_Health(const FGameplayAttributeData& OldHealth);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)
	
	// GAMEPLAY_ATTRIBUTE_DECL(Mana, "Vital Attributes")
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes") FGameplayAttributeData Mana; UFUNCTION() void OnRep_Mana(const FGameplayAttributeData& OldMana);
	GAMEPLAY_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
};
