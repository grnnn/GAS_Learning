// Copyright Taylor Owen-Milner


#include "Interaction/CombatInterface.h"

#include "GameplayTagContainer.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.
int32 ICombatInterface::GetCombatLevel()
{
	return 0;
}

TArray<FGameplayTag> ICombatInterface::GetResistances()
{
	return {};
}
