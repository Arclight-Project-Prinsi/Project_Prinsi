// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prinsi/Component/EntityComponent.h"		// Entity组件
#include "TowerBase.generated.h"

UCLASS()
class PROJECT_PRINSI_API ATowerBase : public AActor {
	GENERATED_BODY()

public:
	ATowerBase();

protected:
	virtual void BeginPlay() override;


	// ~~Entity初期配置
protected:
	// 读取配置表进行初始化
	bool InitFromConfig(FName EntityId);

	// 读取扩展表进行Tower初始化
	bool InitTowerFromConfig(const FEntityTowerExtraConfig* TowerConfig);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UEntityComponent* EntityComp_;		// 创建Entity组件

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity")
	UDataTable* EntityBaseTable_;			// 实体主表

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity|Tower")
	UDataTable* TowerExtraTable_;			// Tower类拓展配置表


	// ~~Status
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Tower")
	float Damage_ = 1.0f;			// 伤害

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Tower")
	int32 BuildCost_ = 42;			// 建造花费
};
