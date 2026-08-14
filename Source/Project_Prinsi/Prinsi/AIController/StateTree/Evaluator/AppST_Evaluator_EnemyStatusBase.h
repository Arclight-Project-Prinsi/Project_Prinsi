// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "AppST_Evaluator_EnemyStatusBase.generated.h"


//@sc Evaluator可理解为ST运行时的一个动态黑板

/**
 *
 */
USTRUCT()
struct FAppST_Evaluator_EnemyStatusInstanceData
{
	GENERATED_BODY()

	//@sc 声明为input以便在资产中进行上下文绑定		
	UPROPERTY(EditAnywhere, Category = "STEnemy|Status", meta = (Input))
	TObjectPtr<APawn> Pawn = nullptr;

	//@sc meta = (Output)允许其他节点（STT?）绑定读取
	UPROPERTY(VisibleAnywhere, Category = "STEnemy|Status", meta = (Output))
	TObjectPtr<AAppTowerBase> Blocker = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "STEnemy|Status", meta = (Output))
	bool bIsBlocked = false;		//@sc 因为是用于ST的标识，直接在Evaluator中更新即可？
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
	void UpdateEnemyStatus(FStateTreeExecutionContext& Context) const;		//@sc 更新用
};