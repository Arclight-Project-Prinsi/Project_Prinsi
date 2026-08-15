// Fill out your copyright notice in the Description page of Project Settings.

#include "Prinsi/AIController/StateTree/Evaluator/AppST_Evaluator_EnemyStatusBase.h"
// Prinsi Define
#include "Prinsi/Define/AppDefineDebug.h"							// Debug工具
#include "Prinsi/Entity/Character/Enemy/AppEnemyCharacterBase.h"
// Misc
#include "StateTreeExecutionContext.h"	
#include "AppST_Evaluator_EnemyStatusBase.h"


void FAppST_Evaluator_EnemyStatusBase::TreeStart(FStateTreeExecutionContext& Context) const
{
	UpdateEnemyStatus(Context);
}

void FAppST_Evaluator_EnemyStatusBase::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	UpdateEnemyStatus(Context);
}

void FAppST_Evaluator_EnemyStatusBase::UpdateEnemyStatus(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& ID = Context.GetInstanceData(*this);
	AAppEnemyCharacterBase* Enemy = Cast<AAppEnemyCharacterBase>(ID.Pawn);
	if (!Enemy)
	{
		ID.Blocker = nullptr;
		return;
	}

	ID.Blocker = Enemy->GetBlocker();
}
