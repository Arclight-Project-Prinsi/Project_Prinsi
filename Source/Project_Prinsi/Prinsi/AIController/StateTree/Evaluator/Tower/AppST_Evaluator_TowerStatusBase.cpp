// Fill out your copyright notice in the Description page of Project Settings.

#include "Prinsi/AIController/StateTree/Evaluator/Tower/AppST_Evaluator_TowerStatusBase.h"
// Prinsi Define
#include "Prinsi/Define/AppDefineDebug.h"							// Debug工具
// Misc
#include "StateTreeExecutionContext.h"	
#include "AppST_Evaluator_TowerStatusBase.h"


void FAppST_Evaluator_TowerStatusBase::TreeStart(FStateTreeExecutionContext& Context) const
{
	UpdateTowerStatus(Context);
}

void FAppST_Evaluator_TowerStatusBase::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	UpdateTowerStatus(Context);
}

void FAppST_Evaluator_TowerStatusBase::UpdateTowerStatus(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& ID = Context.GetInstanceData(*this);

	if (!ID.Tower)
	{
		ID.BlockedEnemy = nullptr;
		return;
	}

	ID.BlockedEnemy = ID.Tower->GetBlockedEnemy();

	//@ch
	if (ID.BlockedEnemy)
	{
		APP_SCR_ERROR(TEXT("存在阻挡敌人!"));
	}
}
