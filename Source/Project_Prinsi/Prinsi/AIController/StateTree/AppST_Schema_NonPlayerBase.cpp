// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/AIController/StateTree/AppST_Schema_NonPlayerBase.h"
#include "Prinsi/AIController/StateTree/Task\AppST_Task_NonPlayerBase.h"	
#include "Prinsi/AIController/StateTree/AppST_Condition_HasTag.h"			


bool UAppST_Schema_NonPlayerBase::IsStructAllowed(const UScriptStruct* InScriptStruct) const
{
	if (!InScriptStruct)
	{
		return false;
	}

	// 放行NonplayerBase的STT
	if (InScriptStruct->IsChildOf(FAppST_Task_NonPlayerBase::StaticStruct()))
	{
		return true;
	}

	// 放行HasTag的Condition
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
