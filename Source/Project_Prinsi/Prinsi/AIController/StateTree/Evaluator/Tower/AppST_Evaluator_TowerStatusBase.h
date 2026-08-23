// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "AppST_Evaluator_TowerStatusBase.generated.h"

class AAppTowerBase;
class AAppEnemyCharacterBase;


USTRUCT()
struct FAppST_Evaluator_TowerStatusInstanceData
{
	GENERATED_BODY()

	// @memo meta = (Input)允许在资产中进行上下文绑定		
	UPROPERTY(EditAnywhere, Category = "StateTree|TowerStatus", meta = (Input))
	TObjectPtr<AAppTowerBase> Tower = nullptr;

	// @memo meta = (Output)允许其他节点（STT）绑定读取
	UPROPERTY(VisibleAnywhere, Category = "StateTree|TowerStatus", meta = (Output))
	TObjectPtr<AAppEnemyCharacterBase> BlockedEnemy = nullptr;	
};

USTRUCT(meta = (DisplayName = "AppST Tower State Base"))
struct PROJECT_PRINSI_API FAppST_Evaluator_TowerStatusBase : public FStateTreeEvaluatorBase
{
	GENERATED_BODY()

	using FInstanceDataType = FAppST_Evaluator_TowerStatusInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	virtual void TreeStart(FStateTreeExecutionContext& Context) const override;
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

protected:
	void UpdateTowerStatus(FStateTreeExecutionContext& Context) const;		
};