// Copyright Taylor Owen-Milner


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaults(ECharacterClass Class) const
{
	return ClassDefaults.FindChecked(Class);
}
