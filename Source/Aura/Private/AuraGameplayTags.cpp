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


	GameplayTags.Attributes_Resistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Resistance"),
			FString("Resistances to various damage types.")
		);
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Resistance.Fire"),
			FString("Fire resistance.")
		);
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Resistance.Lightning"),
			FString("Lightning resistance.")
		);
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Resistance.Arcane"),
			FString("Arcane resistance.")
		);
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Resistance.Physical"),
			FString("Physical resistance.")
		);

	GameplayTags.Input_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Input.LMB"),
			FString("Left mouse button input.")
		);
	GameplayTags.Input_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Input.RMB"),
			FString("Right mouse button input.")
		);
	GameplayTags.Input_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Input.1"),
			FString("1 key input.")
		);
	GameplayTags.Input_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Input.2"),
			FString("2 key input.")
		);
	GameplayTags.Input_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Input.3"),
			FString("3 key input.")
		);
	GameplayTags.Input_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Input.4"),
			FString("4 key input.")
		);

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Damage"),
			FString("tracks damage")
		);
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Damage.Fire"),
			FString("Fire Damage Type")
		);
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Damage.Lightning"),
			FString("Lightning Damage Type")
		);
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Damage.Arcane"),
			FString("Arcane Damage Type")
		);
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Damage.Physical"),
			FString("Physical Damage Type")
		);

	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);

	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Effects.HitReact"),
			FString("Hit stun effects.")
		);
}
