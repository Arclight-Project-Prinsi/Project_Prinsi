// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/GameplayAbility/AppCharacterAttributeSetBase.h"


UAppCharacterAttributeSetBase::UAppCharacterAttributeSetBase()
{
	// ~Žn‰»Attribute
	MaxHealth = 42.0f;
	Health = GetMaxHealth();

	MaxMana = 84.0f;
	Mana = GetMaxMana();
}
