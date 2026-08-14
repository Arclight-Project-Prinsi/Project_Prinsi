// Fill out your copyright notice in the Description page of Project Settings.

#include "Prinsi/AIController/StateTree/Evaluator/AppST_Evaluator_EnemyStatusBase.h"
// Prinsi Define
#include "Prinsi/Define/AppDefineDebug.h"				// Debug工具
#include "Prinsi/Entity/Character/Enemy/AppEnemyCharacterBase.h"
#include "StateTreeExecutionContext.h"	//@sc
#include "AppST_Evaluator_EnemyStatusBase.h"

void FAppST_Evaluator_EnemyStatusBase::TreeStart(FStateTreeExecutionContext& Context) const
{
	//@sc 保持更新?
	UpdateEnemyStatus(Context);
}

void FAppST_Evaluator_EnemyStatusBase::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	UpdateEnemyStatus(Context);
}

void FAppST_Evaluator_EnemyStatusBase::UpdateEnemyStatus(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& ID = Context.GetInstanceData(*this);

	// @sc 上下文从哪来的？
	//AAppEnemyCharacterBase* Enemy = Cast<AAppEnemyCharacterBase>(Context.GetOwner());
	AAppEnemyCharacterBase* Enemy = Cast<AAppEnemyCharacterBase>(ID.Pawn);

	if (!Enemy)
	{
		ID.Blocker = nullptr;
		ID.bIsBlocked = false;
		return;
	}

	ID.Blocker = Enemy->GetBlocker();
	ID.bIsBlocked = IsValid(ID.Blocker);
}
