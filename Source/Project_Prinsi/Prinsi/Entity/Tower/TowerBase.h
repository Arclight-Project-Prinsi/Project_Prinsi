// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prinsi/Entity/EntityBase.h"
#include "TowerBase.generated.h"


struct FEntityTowerExtraConfig;


UCLASS()
class PROJECT_PRINSI_API ATowerBase : public AEntityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity")
	UDataTable* EntityBaseTable;					// 实体主表

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity|Tower")
	UDataTable* TowerExtraTable;					// Tower类拓展配置表

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Tower")
	float Damage = 1.0f;		// 伤害

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Tower")
	int32 BuildCost = 42;		// 建造花费

protected:
	virtual void BeginPlay() override;

protected:
	virtual void InitFromConfig()override;									// 读取配置表实现Entity初始化
	void InitTowerFromConfig(const FEntityTowerExtraConfig* TowerConfig);	// 读取配置表实现Tower初始化
};
