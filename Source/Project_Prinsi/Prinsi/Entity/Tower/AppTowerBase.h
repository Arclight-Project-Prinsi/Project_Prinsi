// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Prinsi/Component/EntityComponent.h"		// Actor Component_实体组件
#include "GameFramework/Actor.h"
#include "AppTowerBase.generated.h"

// ~~前向声明
class UEntityComponent;		// Component_实体组件


UCLASS()
class PROJECT_PRINSI_API AAppTowerBase : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAppTowerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TowerBase|Actor Component")
	TObjectPtr<UEntityComponent> EntityComp;		// Actor Component_实体组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TowerBase|Scene Component")
	TObjectPtr<UStaticMeshComponent> TowerMeshComp;


	//――――――――――――――――――――
	// Entity初期配置
	//――――――――――――――――――――
protected:
	// 读取配置表进行初始化
	bool InitFromConfig(FName EntityId);

	// 读取扩展表进行Tower初始化
	bool InitTowerFromConfig(const FEntityTowerExtraConfig* TowerConfig);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TowerBase|Entity|Config")
	TObjectPtr<UDataTable> EntityBaseTable;			// 实体主表

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TowerBase|Entity|Config")
	TObjectPtr<UDataTable> TowerExtraTable;			// Tower类拓展配置表





	//ws-------------------------------------
	// @todo 属性字段转至GAS处理
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Config Entity|Tower")
	float Damage = -1.0f;				// 伤害

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config Entity|Tower")
	int32 BuildCost = 999;				// 建造花费

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config Entity|Tower")
	FIntPoint FootprintSize = { -1,-1 };	// 占地

	//ws2-------------------------------------
	//[p]部署测试

public:
	FIntPoint GetFootprintSize() { return  FootprintSize; }		// Getter_部署尺寸

	//ws3---------------------------------------
public:
	bool InitialTower();		//[p]初始化函数

protected:
	UPROPERTY(VisibleAnywhere, Category = "Status Tower")
	bool bIsActive = false;		//[p]Active
};