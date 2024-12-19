// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* Config, UserClass* Object,
							PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
	{
		check(Config);

		for (auto Action : Config->AbilityInputActions)
		{
			if (not Action.Action || not Action.Tag.IsValid())
				continue;

			if (PressedFunc)
			{
				BindAction(Action.Action, ETriggerEvent::Started, Object, PressedFunc, Action.Tag);
			}
			if (ReleasedFunc)
			{
				BindAction(Action.Action, ETriggerEvent::Completed, Object, ReleasedFunc, Action.Tag);
			}
			if (HeldFunc)
			{
				BindAction(Action.Action, ETriggerEvent::Triggered, Object, HeldFunc, Action.Tag);
			}
		}
	}
	
};
