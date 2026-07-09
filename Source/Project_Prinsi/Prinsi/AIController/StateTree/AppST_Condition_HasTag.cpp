// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi\AIController\StateTree\AppST_Condition_HasTag.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "StateTreeExecutionContext.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"	// @note
#include "AIController.h"


bool FAppST_Condition_HasTag::TestCondition(FStateTreeExecutionContext& Context) const
{
	// note *this
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

	// @note AbilitySystemGlobals?
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);
	if (!ASC)
	{
		return false;
	}

	//-------------------------------

	FGameplayTagContainer TC;
	ASC->GetOwnedGameplayTags(TC);		// @note 取出对应角色ASC上的Container

	// @note Exact?
	if (InstanceData.bExactMatch)
	{
		return TC.HasTagExact(InstanceData.TargetTag);
	}

	// @note 
	return TC.HasTag(InstanceData.TargetTag);
}
