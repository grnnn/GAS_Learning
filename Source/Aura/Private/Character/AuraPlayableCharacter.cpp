// Copyright Taylor Owen-Milner


#include "Character/AuraPlayableCharacter.h"

#include "AbilitySystemComponent.h"
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

	SetAbilitySystemPointers();
	InitOverlay();
}

void AAuraPlayableCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	SetAbilitySystemPointers();
}

void AAuraPlayableCharacter::SetAbilitySystemPointers()
{
	auto AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	check(AuraPlayerState);
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
    AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
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
