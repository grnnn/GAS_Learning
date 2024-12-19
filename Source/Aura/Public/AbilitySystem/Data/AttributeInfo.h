// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag Tag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;

	UPROPERTY(BlueprintReadOnly)
	float Value = 0.f;
};

/**
 * 
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	FAuraAttributeInfo GetAttributeInfo(const FGameplayTag& Tag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FAuraAttributeInfo> AttributeInfo;
};
