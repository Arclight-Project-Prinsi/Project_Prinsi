// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Components/StateTreeAIComponentSchema.h"
#include "AppST_Schema_NonPlayerBase.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT_PRINSI_API UAppST_Schema_NonPlayerBase : public UStateTreeAIComponentSchema
{
	GENERATED_BODY()
	
public:
	// 放行struct设定
	virtual bool IsStructAllowed(const UScriptStruct* InScriptStruct)const override;

	// 放行class设定
	virtual bool IsClassAllowed(const UClass* InClass)const override;
};
