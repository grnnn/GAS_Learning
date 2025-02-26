// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"

#include "AuraEnemy.generated.h"

class AAuraAIController;
class UBehaviorTree;
class UWidgetComponent;
class UEnemyWidgetController;

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();

	// AActor
	virtual void PossessedBy(AController* NewController) override;

	// IEnemyInterface
	virtual void Highlight() override;
	virtual void UnHighlight() override;
	virtual void SetCombatTarget_Implementation(AActor* InTarget) override;
	virtual AActor* GetCombatTarget_Implementation() override;
	virtual void SetCombatSocketTag_Implementation(FGameplayTag InTag) override;
	virtual FGameplayTag GetCombatSocketTag_Implementation() override;

	// ICombatInterface
	virtual int32 GetCombatLevel() override;
	virtual void Die() override;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float DeathLifespan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FGameplayTag CombatSocketTag;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultData() override;
	virtual void HandleDeath() override;

	void CreateEnemyWidgetController();
	void RegisterGameplayTagEvents();
	void SetDefaultBlackboardValues();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UEnemyWidgetController> EnemyWidgetController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass Class = ECharacterClass::Elementalist;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AiController;
};
