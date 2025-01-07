// Copyright Taylor Owen-Milner


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	auto MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	if (not AbilitySystemComponent.IsValid())
		return;

	if (auto Asc = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (Asc->AbilityActorInfo->PlayerController.IsValid())
		{
			FHitResult HitResult;
			Asc->AbilityActorInfo->PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
			ValidData.Broadcast(HitResult.Location);
		}
	}
}
