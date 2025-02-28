// Copyright Taylor Owen-Milner


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAuraProjectile::AAuraProjectile()
{
	bReplicates = true;
	bAlwaysRelevant = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("Root");
	SetRootComponent(CollisionSphere);
	CollisionSphere->SetCollisionObjectType(ECC_Projectile);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(Lifespan);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);

	if (LoopingSound)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, RootComponent);
	}

	SpinDirection = FMath::VRand() * RotationSpeed;
}

void AAuraProjectile::Destroyed()
{
	if (not bHit && not HasAuthority())
	{
		Impact();
	}
	
	Super::Destroyed();
}

void AAuraProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSpinning)
	{
		SetActorRotation(GetActorRotation() + FRotator(
			SpinDirection.X * DeltaTime,
			SpinDirection.Y * DeltaTime,
			SpinDirection.Z * DeltaTime));
	}
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Context = DamageEffect.Data.IsValid() ? DamageEffect.Data.Get()->GetContext() : FGameplayEffectContextHandle();
	if (Context.GetEffectCauser() == OtherActor || not OtherActor->HasAuthority())
	{
		return;
	}
	
	Impact();
	
	if (HasAuthority() && OtherActor->HasAuthority())
	{
		TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (TargetAsc)
		{
			FScopedPredictionWindow ScopedPrediction(TargetAsc);
			TargetAsc->ApplyGameplayEffectSpecToSelf(*DamageEffect.Data.Get());//, TargetAsc->ScopedPredictionKey);
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

void AAuraProjectile::Impact()
{
	if (not bHit && ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	}
	if (ImpactFx)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactFx, GetActorLocation());
	}
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
	}
}

