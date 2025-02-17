// Copyright Taylor Owen-Milner


#include "Interaction/CombatInterface.h"

#include "GameplayTagContainer.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.
int32 ICombatInterface::GetCombatLevel()
{
	return 0;
}

FVector ICombatInterface::GetCombatSocketLocation()
{
	return FVector();
}

TArray<FGameplayTag> ICombatInterface::GetResistances()
{
	return {};
}

void ICombatInterface::SetFacingLocation(const FVector& Location)
{
}

UAnimMontage* ICombatInterface::GetHitReactMontage()
{
	return nullptr;
}
