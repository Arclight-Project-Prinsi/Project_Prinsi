// Fill out your copyright notice in the Description page of Project Settings.

#include "Prinsi/AIController/StateTree/AppST_Condition_HasTag.h"
#include "Prinsi/Define/AppDefineDebug.h"	
#include "StateTreeExecutionContext.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"		// @need UAbilitySystemGlobals::GetAbilitySystemComponentFromActor
#include "AIController.h"


bool FAppST_Condition_HasTag::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!InstanceData.TargetTag.IsValid())
	{
		return false;
	}

	AAIController* AIC = Cast<AAIController>(Context.GetOwner());
	if (!AIC)
	{
		return false;
	}

	APawn* Pawn = AIC->GetPawn();
	if (!Pawn)
	{
		return false;
	}

	// 从ST使用对象上获取ASC
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);
	if (!ASC)
	{
		return false;
	}

	// 从目标ASC中获取TagContainer
	FGameplayTagContainer TC;
	ASC->GetOwnedGameplayTags(TC);		

	// @memo bExactMatch_完全一致，并不仅仅是上层级一致
	if (InstanceData.bExactMatch)
	{
		return TC.HasTagExact(InstanceData.TargetTag);
	}

	// 仅上层级一致即可
	return TC.HasTag(InstanceData.TargetTag);
}
