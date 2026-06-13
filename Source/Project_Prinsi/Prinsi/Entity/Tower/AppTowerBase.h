// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"

#include "Prinsi/Component/EntityComponent.h"		// Component_实体组件

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


	//ws0--------------------------------------------
	//――――――――――――――――――――
	// Entity初期配置
	//――――――――――――――――――――
protected:
	// 读取配置表进行初始化
	bool InitFromConfig(FName EntityId);

	// 读取扩展表进行Tower初始化
	bool InitTowerFromConfig(const FEntityTowerExtraConfig* TowerConfig);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor Components")
	UEntityComponent* EntityComp;			// 创建Entity组件

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Tower|Entity")
	UDataTable* EntityBaseTable;			// 实体主表

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Tower|Entity")
	UDataTable* TowerExtraTable;			// Tower类拓展配置表

	//ws-------------------------------------
	// ~~Status
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Tower")
	float Damage = 1.0f;			// 伤害

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Tower")
	int32 BuildCost = 42;			// 建造花费

	//ws2-------------------------------------
	//[p]部署测试
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scene Component")
	TObjectPtr<UStaticMeshComponent> TowerMeshComp;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entity Config|Tower Config")
	FIntPoint FootprintSize = { 2,2 };		// 部署尺寸

public:
	FIntPoint GetFootprintSize() { return  FootprintSize; }		// Getter_部署尺寸
};
