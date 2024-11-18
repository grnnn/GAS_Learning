// Copyright Taylor Owen-Milner


#include "Character/AuraPlayableCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"

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
}

void AAuraPlayableCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	SetAbilitySystemPointers();
}

void AAuraPlayableCharacter::SetAbilitySystemPointers()
{
	auto AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
    AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
    AttributeSet = AuraPlayerState->GetAttributeSet();
}
