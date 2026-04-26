// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prinsi/Entity/EntityBase.h"
#include "TowerBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_PRINSI_API ATowerBase : public AEntityBase
{
	GENERATED_BODY()
	
	//ws------------------------------------
public:
	virtual void InitFromConfig() override;			// 读取配置表实现初始化
};
