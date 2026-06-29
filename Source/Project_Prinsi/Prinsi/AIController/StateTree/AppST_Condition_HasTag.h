// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "StateTreeConditionBase.h"
#include "AppST_Condition_HasTag.generated.h"


/**
 *
 */
USTRUCT()
struct FAppST_Condition_HasGameplayTagInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Condition")
	FGameplayTag TargetTag;

	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bExactMatch = false;
};

USTRUCT()
struct PROJECT_PRINSI_API FAppST_Condition_HasTag : public FStateTreeConditionBase
{
	GENERATED_BODY()

	// @note 必须要有这么个设定?
	using FInstanceDataType = FAppST_Condition_HasGameplayTagInstanceData;

	// @note 拿到该Condition的构造体类型?
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	// @note 如果符合这种情况?
	virtual bool TestCondition(FStateTreeExecutionContext& Context)const override;
};
