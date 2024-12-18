// Copyright Taylor Owen-Milner


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::Initialize()
{
	
    GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Attributes.Primary.Strength"),
    		FString("Primary attribute representing physical power.")
    	);
    GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Attributes.Primary.Intelligence"),
    		FString("Primary attribute representing magical power.")
    	);
    GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Attributes.Primary.Dexterity"),
    		FString("Primary attribute representing agility and reflexes.")
    	);
    GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Attributes.Primary.Vigor"),
    		FString("Primary attribute representing endurance and stamina.")
    	);
    
    GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Attributes.Secondary.Armor"),
    		FString("Determines damage reduction from physical attacks.")
    	);
    GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Secondary.ArmorPenetration"),
			FString("Determines how much armor to ignore.")
		);
    GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Attributes.Secondary.BlockChance"),
    		FString("Determines the chance to block attacks.")
    	);
    GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Attributes.Secondary.CriticalHitChance"),
    		FString("Determines the chance to land a critical hit.")
    	);
    GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Attributes.Secondary.CriticalHitDamage"),
    		FString("Determines the bonus damage after a critical hit is scored.")
    	);
    GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Attributes.Secondary.CriticalHitResistance"),
    		FString("Reduces critical hit chance of attacking enemies.")
    	);
    GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Attributes.Secondary.HealthRegeneration"),
    		FString("Determines the rate of health regeneration.")
    	);
    GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Attributes.Secondary.ManaRegeneration"),
    		FString("Determines the rate of mana regeneration.")
    	);
    GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Attributes.Secondary.MaxHealth"),
    		FString("Determines the maximum health.")
    	);
    GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Attributes.Secondary.MaxMana"),
    		FString("Determines the maximum mana.")
    	);
}
