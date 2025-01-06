// Copyright Taylor Owen-Milner


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"

#include "Interaction/CombatInterface.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (not HasAuthority(&ActivationInfo))
		return;

	auto CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (not CombatInterface)
		return;
	
	const FVector CombatLocation = CombatInterface->GetCombatSocketLocation();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(CombatLocation);
	// TODO: Set the rotation of the SpawnTransform to face the target
	auto Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform,
		GetOwningActorFromActorInfo(),Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	// TODO: Give the Projectile a GE Spec for causing damage

	Projectile->FinishSpawning(SpawnTransform);
}
