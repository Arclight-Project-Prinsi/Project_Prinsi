// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/AIController/StateTree/AppST_Schema_NonPlayerBase.h"
#include "Prinsi/AIController/StateTree/Task\AppST_Task_NonPlayerBase.h"	
#include "Prinsi/AIController/StateTree/Evaluator/AppST_Evaluator_EnemyStatusBase.h"	//@sc
#include "Prinsi/AIController/StateTree/AppST_Condition_HasTag.h"			


bool UAppST_Schema_NonPlayerBase::IsStructAllowed(const UScriptStruct* InScriptStruct) const
{
	if (!InScriptStruct)
	{
		return false;
	}

	//~~ Task放行 ~~//
	if (InScriptStruct->IsChildOf(FAppST_Task_NonPlayerBase::StaticStruct()))
	{
		return true;
	}

	//~~ Evaluator放行 ~~//
	if (InScriptStruct->IsChildOf(FAppST_Evaluator_EnemyStatusBase::StaticStruct()))
	{
		return true;
	}

	//~~ Condition放行 ~~//
	if (InScriptStruct->IsChildOf(FAppST_Condition_HasTag::StaticStruct()))
	{
		return true;
	}

	return Super::IsStructAllowed(InScriptStruct);
}

bool UAppST_Schema_NonPlayerBase::IsClassAllowed(const UClass* InClass) const
{
	return false;
}
