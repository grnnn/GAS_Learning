// Copyright Taylor Owen-Milner


#include "AbilitySystem/AuraAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "AuraGameplayTags.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Interaction/CombatInterface.h"

#include "Kismet/GameplayStatics.h"

#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(Strength, Primary)
	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(Intelligence, Primary)
	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(Dexterity, Primary)
	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(Vigor, Primary)

	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(Armor, Secondary)
	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(ArmorPenetration, Secondary)
	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(BlockChance, Secondary)
	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(CriticalHitChance, Secondary)
	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(CriticalHitDamage, Secondary)
	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(CriticalHitResistance, Secondary)
	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(HealthRegeneration, Secondary)
	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(ManaRegeneration, Secondary)
	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(MaxHealth, Secondary)
	GAMEPLAY_ATTRIBUTE_TAG_MAPPING(MaxMana, Secondary)
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// primary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// secondary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// vital
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	else if (Attribute == GetArmorAttribute() ||
			 Attribute == GetArmorPenetrationAttribute() ||
			 Attribute == GetBlockChanceAttribute() ||
			 Attribute == GetCriticalHitChanceAttribute() ||
			 Attribute == GetCriticalHitResistanceAttribute() ||
			 Attribute == GetHealthRegenerationAttribute() ||
			 Attribute == GetManaRegenerationAttribute() ||
			 Attribute == GetMaxHealthAttribute() ||
			 Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
		NewValue = FMath::Floor(NewValue);
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.Source = {};
	Props.Target = {};
	
	auto SourceAsc = Props.EffectContextHandle.GetInstigatorAbilitySystemComponent();
	Props.Source.AbilitySystemComponent = SourceAsc;

	if (IsValid(SourceAsc) && SourceAsc->AbilityActorInfo.IsValid())
	{
		if (SourceAsc->AbilityActorInfo->AvatarActor.IsValid())
		{
			Props.Source.Actor = SourceAsc->AbilityActorInfo->AvatarActor.Get();
		}

		AController* Controller = nullptr;
		if (SourceAsc->AbilityActorInfo->PlayerController.IsValid())
		{
			Controller = SourceAsc->AbilityActorInfo->PlayerController.Get();
			Props.Source.Controller = Controller;
		}
		else if (APawn* Pawn = Cast<APawn>(SourceAsc->AbilityActorInfo->AvatarActor))
		{
			Controller = Pawn->GetController();
			Props.Source.Controller = Controller;
		}
		if (Controller)
		{
			Props.Source.Character = Cast<ACharacter>(Controller->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.Target.Actor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.Target.Controller = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.Target.Character = Cast<ACharacter>(Props.Target.Actor);
		Props.Target.AbilitySystemComponent = Data.Target.AbilityActorInfo->AbilitySystemComponent.Get();
	}
}


void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("Changed Health On %s to %f"), *Props.Target.Actor->GetName(), GetHealth());
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		float NewMaxHealth = FMath::Max(GetMaxHealth(), 0.f);
		NewMaxHealth = FMath::Floor(NewMaxHealth);
		SetMaxHealth(NewMaxHealth);
	}
	else if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
    {
        float NewMaxMana = FMath::Max(GetMaxMana(), 0.f);
        NewMaxMana = FMath::Floor(NewMaxMana);
        SetMaxMana(NewMaxMana);
    }
	else if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		float LocalIncomingDamage = FMath::Floor(GetIncomingDamage());
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0)
		{
			float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			bool bFatal = NewHealth <= 0.f;
			if (bFatal)
			{
				if (auto AsCombatInterface = Cast<ICombatInterface>(Props.Target.Actor))
				{
					AsCombatInterface->Die();
				}
			}
			else
			{
				FGameplayTagContainer Container;
				Container.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				Props.Target.AbilitySystemComponent->TryActivateAbilitiesByTag(Container);
			}

			if (Props.Source.Character != Props.Target.Character)
			{
				auto PlayerController = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(Props.Source.Character, 0));
				if (PlayerController)
				{
					bool bIsCritical = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
					bool bIsBlocked = UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
					PlayerController->ShowDamageNumber(Props.Target.Character, LocalIncomingDamage, bIsCritical, bIsBlocked);
				}
			}
		}
	}
}

GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, Strength)
GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, Intelligence)
GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, Dexterity)
GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, Vigor)

GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, Armor)
GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, ArmorPenetration)
GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, BlockChance)
GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, CriticalHitChance)
GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, CriticalHitDamage)
GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, CriticalHitResistance)
GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, HealthRegeneration)
GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, ManaRegeneration)
GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, MaxHealth)
GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, MaxMana)

GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, Health)
GAMEPLAY_ATTRIBUTE_DEF(UAuraAttributeSet, Mana)
