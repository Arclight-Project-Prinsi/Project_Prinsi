// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "AppST_Evaluator_EnemyStatusBase.generated.h"


/**
 *	@note Evaluator可理解为ST运行时的一个动态黑板	
 */
USTRUCT()
struct FAppST_Evaluator_EnemyStatusInstanceData
{
	GENERATED_BODY()

	// @memo meta = (Input)允许在资产中进行上下文绑定		
	UPROPERTY(EditAnywhere, Category = "STEnemy|Status", meta = (Input))
	TObjectPtr<APawn> Pawn = nullptr;

	// @memo meta = (Output)允许其他节点（STT）绑定读取
	UPROPERTY(VisibleAnywhere, Category = "STEnemy|Status", meta = (Output))
	TObjectPtr<AAppTowerBase> Blocker = nullptr;	
};

USTRUCT(meta = (DisplayName = "AppST Enemy State Base"))
struct PROJECT_PRINSI_API FAppST_Evaluator_EnemyStatusBase : public FStateTreeEvaluatorBase
{
	GENERATED_BODY()

	using FInstanceDataType = FAppST_Evaluator_EnemyStatusInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	virtual void TreeStart(FStateTreeExecutionContext& Context) const override;
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

protected:
	void UpdateEnemyStatus(FStateTreeExecutionContext& Context) const;		
};