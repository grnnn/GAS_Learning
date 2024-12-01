// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply,
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove,
};

USTRUCT(BlueprintType)
struct FGameplayEffectApplication
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDestroyOnEffectApplication = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDestroyOnEffectRemoval = false;
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToActor(AActor* Actor, TSubclassOf<UGameplayEffect> EffectClass, EEffectRemovalPolicy RemovalPolicy = EEffectRemovalPolicy::DoNotRemove);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* Actor);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<FGameplayEffectApplication> GameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	float ActorLevel = 1.f;

private:
	TArray<FActiveGameplayEffectHandle> ActiveOnEndOverlapEffects;
};
