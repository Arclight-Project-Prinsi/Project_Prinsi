// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi\AIController\StateTree\AppST_Schema_NonPlayerBase.h"
#include "Prinsi\AIController\StateTree\Task\AppST_Task_NonPlayerBase.h"


bool UAppST_Schema_NonPlayerBase::IsStructAllowed(const UScriptStruct* InScriptStruct) const
{
	if (!InScriptStruct)
	{
		return false;
	}

	// @todo 加入STT的构造体类型
	if (InScriptStruct->IsChildOf(FAppST_Task_NonPlayerBase::StaticStruct()))
	{
		return true;
	}

	return Super::IsStructAllowed(InScriptStruct);

}

bool UAppST_Schema_NonPlayerBase::IsClassAllowed(const UClass* InClass) const
{
	return false;
}
