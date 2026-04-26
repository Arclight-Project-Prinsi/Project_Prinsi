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

protected:
	virtual void BeginPlay() override;

	//ws------------------------------------
public:
	virtual void InitFromConfig() override;			// 读取配置表实现初始化

	//ws2------------------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity")
	UDataTable* EntityBaseTable;					// 实体主表

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity|Tower")
	UDataTable* TowerExtraTable;					// Tower类拓展配置表
};
