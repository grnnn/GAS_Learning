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
	check(AbilitySystemComponent.IsValid());
	if (Ability->GetCurrentActorInfo() && Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		SendMouseCursorData();	
	}
	else
	{
		auto SpecHandle = GetAbilitySpecHandle();
		auto PredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey)
			.AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey);
		if (not bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	auto SingleTargetHitData = new FGameplayAbilityTargetData_SingleTargetHit();
	if (auto Asc = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		FScopedPredictionWindow ScopedPrediction(Asc);
		
		if (Asc->AbilityActorInfo->PlayerController.IsValid())
		{
			FHitResult HitResult;
			Asc->AbilityActorInfo->PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
			SingleTargetHitData->HitResult = HitResult;
		}

		FGameplayAbilityTargetDataHandle Handle;
		Handle.Add(SingleTargetHitData);
		Asc->ServerSetReplicatedTargetData(
			GetAbilitySpecHandle(),
			GetActivationPredictionKey(),
			Handle,
			FGameplayTag(),
			Asc->ScopedPredictionKey);

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			ValidData.Broadcast(Handle);
		}
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Handle, FGameplayTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(Handle);
	}
}
