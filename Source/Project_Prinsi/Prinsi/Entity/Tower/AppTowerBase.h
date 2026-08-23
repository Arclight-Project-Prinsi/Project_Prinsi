// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
// Prinsi Component
#include "Prinsi/Component/EntityComponent.h"		// 实体组件
// Misc
#include "GameFramework/Actor.h"
#include "AppTowerBase.generated.h"

class UEntityComponent;
class UBoxComponent;
class AAppEnemyCharacterBase;	// @scaff
class USkeletalMeshComponent;	// @SC

UCLASS()
class PROJECT_PRINSI_API AAppTowerBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAppTowerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TowerBase|ActorComponent")
	TObjectPtr<UEntityComponent> EntityComp;					// @sc Actor Component_实体组件(只用于初始化?)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TowerBase|Component")
	TObjectPtr<UBoxComponent> BoxBlockEnemyComp;				// 阻挡检测用Box


	//~~ 阻挡关联（Block） ~~//
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TowerBase|Config|Block")
	int32 MaxBlockCount = 1;		//最大可阻挡数

	UPROPERTY()
	TArray<TObjectPtr<AAppEnemyCharacterBase>> BlockedEnemies;	// 当前阻挡敌人

	//ws-------------------------------------------------
protected:
	//@sc
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TowerBase|Scene Component")
	TObjectPtr<UStaticMeshComponent> TowerMeshComp;*/

	//@sc
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TowerBase|SceneComponent")
	TObjectPtr<USkeletalMeshComponent> MeshComp;

	//ws2-------------------------------------------------
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

	//ws4----------------
	// @scaff 阻挡机制相当于是一种嘲讽

protected:
	UFUNCTION()
	void OnBoxBlockEnemyBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

public:
	bool TryBlockEnemy(AAppEnemyCharacterBase* Enemy);
	void ReleaseEnemy(AAppEnemyCharacterBase* Enemy);
	bool CanBlockEnemy(const AAppEnemyCharacterBase* Enemy) const;
	int32 GetCurrentBlockCount() const;


	//ws5---------------------
	AAppEnemyCharacterBase* GetBlockedEnemy();
};