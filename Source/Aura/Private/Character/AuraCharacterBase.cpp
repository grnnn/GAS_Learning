// Copyright Taylor Owen-Milner


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "Aura/Aura.h"

#include "Components/CapsuleComponent.h"


AAuraCharacterBase::AAuraCharacterBase()
{
 	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	auto AuraAsc = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (not HasAuthority())
		return;

	AuraAsc->AddCharacterAbilities(StartupAbilities);
}

FVector AAuraCharacterBase::GetCombatSocketLocation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level)
{
	check(IsValid(GetAbilitySystemComponent()));
	check(EffectClass);
	
	auto EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	auto EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, Level, EffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultData()
{
	for (auto GameplayEffect : DefaultAttributes)
	{
		ApplyEffectToSelf(GameplayEffect);
	}
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMi))
	{
		UMaterialInstanceDynamic* DynamicMi = UMaterialInstanceDynamic::Create(DissolveMi, this);
		GetMesh()->SetMaterial(0, DynamicMi);
		StartDissolveTimeline(DynamicMi);
	}
	if (IsValid(WeaponDissolveMi))
	{
		UMaterialInstanceDynamic* DynamicMi = UMaterialInstanceDynamic::Create(WeaponDissolveMi, this);
		Weapon->SetMaterial(0, DynamicMi);
		StartWeaponDissolveTimeline(DynamicMi);
	}
}

