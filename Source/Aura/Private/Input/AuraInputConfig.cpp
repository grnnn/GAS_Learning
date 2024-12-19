// Copyright Taylor Owen-Milner


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::GetActionForTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FAuraInputAction& Action : AbilityInputActions)
	{
		if (Action.Tag.MatchesTagExact(Tag))
		{
			return Action.Action;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("No action found for tag %s"), *Tag.ToString());
	}

	return nullptr;
}
