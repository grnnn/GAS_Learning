// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/IHightlightable.h"

#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UEnemyWidgetController;

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IIHightlightable
{
	GENERATED_BODY()
public:
	AAuraEnemy();

	// IIHightlightable
	virtual void Highlight() override;
	virtual void UnHighlight() override;

	// ICombatInterface
	virtual int32 GetCombatLevel() override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() override;

	void CreateEnemyWidgetController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UEnemyWidgetController> EnemyWidgetController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass Class = ECharacterClass::Elementalist;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
};
