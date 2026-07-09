// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/AnimNotify/AppAnimNotify_ComboBranch.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "Prinsi/GameplayAbility/AppGA_AbilityAttackBase.h"
#include "Prinsi/Entity/Character/AppCharacterBase.h"		
#include "AbilitySystemComponent.h"


void UAppAnimNotify_ComboBranch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}

	AAppCharacterBase* Character = Cast<AAppCharacterBase>(MeshComp->GetOwner());
	if (!Character)
	{
		return;
	}

	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	//@todo
	for (FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (!Spec.IsActive())
		{
			continue;
		}

		TArray<UGameplayAbility*> AbilityInstances = Spec.GetAbilityInstances();

		for (UGameplayAbility* AbilityInstance : AbilityInstances)
		{
			UAppGA_AbilityAttackBase* AppAbility = Cast<UAppGA_AbilityAttackBase>(AbilityInstance);
			if (!AppAbility)
			{
				continue;
			}

			AppAbility->TryActivateNextCombo();
			return;
		}
	}
}