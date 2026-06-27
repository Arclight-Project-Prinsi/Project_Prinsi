// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "AppEQSContext_TargetTower.generated.h"


/**
 *
 */
UCLASS()
class PROJECT_PRINSI_API UAppEQSContext_TargetTower : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	// @note
	virtual void ProvideContext(
		FEnvQueryInstance& QueryInstance,
		FEnvQueryContextData& ContextData
	)const override;
};
