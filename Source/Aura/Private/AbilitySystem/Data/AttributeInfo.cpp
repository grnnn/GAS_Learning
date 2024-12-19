// Copyright Taylor Owen-Milner


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::GetAttributeInfo(const FGameplayTag& Tag, bool bLogNotFound) const
{
	auto Attribute = AttributeInfo.Find(Tag);
	if (Attribute)
	{
		return *Attribute;
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Attribute %s not found in AttributeInfo"), *Tag.ToString());
	}
	
	return FAuraAttributeInfo();
}
