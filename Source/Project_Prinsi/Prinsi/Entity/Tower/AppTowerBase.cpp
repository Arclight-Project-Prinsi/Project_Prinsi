// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/Entity/Tower/AppTowerBase.h"


AAppTowerBase::AAppTowerBase() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ~~初始化组件
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	TowerMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	EntityComp = CreateDefaultSubobject<UEntityComponent>(TEXT("Entity"));

	TowerMeshComp->SetupAttachment(RootComponent);
}

void AAppTowerBase::BeginPlay() {
	Super::BeginPlay();

	// ~~通过EntityId进行初始化(主表+扩展表)
	if (!EntityComp) {
		UE_LOG(LogTemp, Warning, TEXT("Entity组件为空_TowerBase.cpp")); return;
	}
	if (!InitFromConfig(EntityComp->GetEntityId())) {
		UE_LOG(LogTemp, Warning, TEXT("通过EntityId进行初始化失败(TowerBase.cpp)")); return;
	}
}

bool AAppTowerBase::InitFromConfig(FName Id) {
	// ~~读取主表
	if (!EntityBaseTable) {
		UE_LOG(LogTemp, Warning, TEXT("EntityBaseTable为空_TowerBase.cpp"));
		return false;
	}

	// ~~通过EntityId找到对应行(Raw)
	const FEntityBaseConfig* BaseConfig =
		EntityBaseTable->FindRow<FEntityBaseConfig>(Id, TEXT("无法找到对应Raw_TowerBase.cpp"));
	if (!BaseConfig) {
		UE_LOG(LogTemp, Warning,
			TEXT("无法找到对应EntityBase配置数据, EntityId = %s_TowerBase.cpp"), *Id.ToString()); return false;
	}

	// ~~确认Entity种类正确
	if (BaseConfig->EntityType != EEntityType::EntityTower) {
		UE_LOG(LogTemp, Warning, TEXT(" EntityType不匹配,EntityId = %s_TowerBase.cpp"), *Id.ToString()); return false;
	}

	// ~~Entity组件初始化
	if (!EntityComp->InitFromConfig(BaseConfig)) {
		UE_LOG(LogTemp, Warning, TEXT("Entity组件初始化失败_TowerBase.cpp")); return false;
	}

	// ~~读取Tower扩展表(Tower)
	if (!TowerExtraTable) {
		UE_LOG(LogTemp, Warning, TEXT("TowerExtraTable为空_TowerBase.cpp")); return false;
	}

	// ~~通过EntityId找到对应行(Raw)
	const FEntityTowerExtraConfig* TowerConfig =
		TowerExtraTable->FindRow<FEntityTowerExtraConfig>(Id, TEXT("Lookup TowerExtra"));
	if (!TowerConfig) {
		UE_LOG(LogTemp, Warning,
			TEXT("无法找到对应Tower配置数据, EntityId = %s_TowerBase.cpp"), *Id.ToString()); return false;
	}

	// ~~Tower字段初始化
	if (!InitTowerFromConfig(TowerConfig)) {
		UE_LOG(LogTemp, Warning, TEXT("Tower扩展字段初始化失败_TowerBase.cpp")); return false;
	}

	return true;
}

bool AAppTowerBase::InitTowerFromConfig(const FEntityTowerExtraConfig* TowerConfig) {
	if (!TowerConfig) { return false; }

	Damage = TowerConfig->Damage;
	BuildCost = TowerConfig->BuildCost;
	return true;
}

