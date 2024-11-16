// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

struct FInputActionValue;
class IIHightlightable;
class UInputMappingContext;
class UInputAction;


UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	// fncs
	void Move(const FInputActionValue& Value);
	void CursorTrace();

	// properties
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	// member vars
	TScriptInterface<IIHightlightable> LastCursorHit;
	TScriptInterface<IIHightlightable> CurrentCursorHit;
};


