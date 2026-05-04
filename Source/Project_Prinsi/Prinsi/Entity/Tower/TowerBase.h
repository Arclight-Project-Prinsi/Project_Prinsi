// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prinsi/Component/EntityComponent.h"		// Entity组件
#include "TowerBase.generated.h"

UCLASS()
class PROJECT_PRINSI_API ATowerBase : public AActor
{
	GENERATED_BODY()

public:
	ATowerBase();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UEntityComponent* EntityComponent_;		// 创建Entity组件

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity")
	UDataTable* EntityBaseTable_;			// 实体主表

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity|Tower")
	UDataTable* TowerExtraTable_;			// Tower类拓展配置表

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Tower")
	float Damage_ = 1.0f;			// 伤害

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Tower")
	int32 BuildCost_ = 42;			// 建造花费

protected:
	// 读取配置表实现Tower初始化
	void InitTowerFromConfig(const FEntityTowerExtraConfig* TowerConfig);	
};
