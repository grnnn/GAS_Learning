// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class AComponentPoolContainer;
class UDamageTextComponent;
class UAuraInputConfig;
struct FInputActionValue;
class IIHightlightable;
class UInputMappingContext;
class UInputAction;
class UAuraAbilitySystemComponent;
class USplineComponent;


UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(ACharacter* TargetCharacter, float Damage, bool bIsCritical, bool bIsBlocked, bool bIsResisted);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void FreeDamageComponent(UDamageTextComponent* DamageText);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	// fncs
	void Move(const FInputActionValue& Value);
	void ShiftPressed() { bShiftKeyPressed = true; }
	void ShiftReleased() { bShiftKeyPressed = false; }
	void CursorTrace();
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	UAuraAbilitySystemComponent* GetAsc();
	void AutoRun();

	// properties
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	// member vars
	TScriptInterface<IIHightlightable> LastCursorHit;
	TScriptInterface<IIHightlightable> CurrentCursorHit;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	FVector CachedDestination;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.0f;
	FHitResult CursorHit;
	bool bShiftKeyPressed = false;
	AComponentPoolContainer* DamageTextPoolContainer = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> MovementSpline;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AComponentPoolContainer> DamageTextPoolClass;
};


