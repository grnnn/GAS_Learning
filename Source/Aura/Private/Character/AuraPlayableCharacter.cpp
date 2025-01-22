// Copyright Taylor Owen-Milner


#include "Character/AuraPlayableCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraPlayableCharacter::AAuraPlayableCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraPlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraPlayableCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

int32 AAuraPlayableCharacter::GetCombatLevel()
{
	auto AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetCombatLevel();
}

void AAuraPlayableCharacter::SetFacingLocation(const FVector& Location)
{
	if (UFunction* SetFacingTargetFunction = FindFunction(TEXT("SetFacingTarget")))
	{
		FVector TargetLocation = Location;
		ProcessEvent(SetFacingTargetFunction, &TargetLocation);
	}
}

void AAuraPlayableCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	SetAbilitySystemPointers();
	InitOverlay();
	InitializeDefaultData();
}

void AAuraPlayableCharacter::SetAbilitySystemPointers()
{
	auto AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	check(AuraPlayerState);
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
    AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
    AttributeSet = AuraPlayerState->GetAttributeSet();
}

void AAuraPlayableCharacter::InitOverlay()
{
	auto AuraPlayerController = Cast<AAuraPlayerController>(GetController());
	if (!AuraPlayerController)
		return;

	auto HUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD());
	if (!HUD)
		return;

	auto AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	check(AuraPlayerState);

	HUD->InitOverlay(AuraPlayerController, AuraPlayerState, GetAbilitySystemComponent(), GetAttributeSet());
	
}
