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
	// @note 决定该Schema允许的构造体
	virtual bool IsStructAllowed(const UScriptStruct* InScriptStruct)const override;

	// @note 决定该Schema允许的Class
	virtual bool IsClassAllowed(const UClass* InClass)const override;
};
