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
}

void UAuraProjectileSpell::FireProjectile(const FGameplayAbilityActivationInfo ActivationInfo, const FVector& TargetLocation)
{
	if (not HasAuthority(&ActivationInfo))
		return;

	auto CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (not CombatInterface)
		return;
	
	const FVector CombatLocation = CombatInterface->GetCombatSocketLocation();
	FRotator Rotation = (TargetLocation - CombatLocation).Rotation();
	Rotation.Pitch = 0; // We don't want the projectile to be aimed up or down
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(CombatLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	auto Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform,
		GetOwningActorFromActorInfo(),Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	// TODO: Give the Projectile a GE Spec for causing damage

	Projectile->FinishSpawning(SpawnTransform);
}
