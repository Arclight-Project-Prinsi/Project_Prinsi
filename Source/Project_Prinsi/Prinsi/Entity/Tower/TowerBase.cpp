// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/Entity/Tower/TowerBase.h"


ATowerBase::ATowerBase() {
	PrimaryActorTick.bCanEverTick = true;

	// ~~初始化组件
	EntityComponent_ = CreateDefaultSubobject<UEntityComponent>(TEXT("EntityComponent"));
}

void ATowerBase::BeginPlay() {
	Super::BeginPlay();

	// ~~通过EntityId进行初始化(主表+扩展表)
	if (!EntityComponent_) {
		UE_LOG(LogTemp, Warning, TEXT("Entity组件为空(TowerBase.cpp)")); return;
	}
	if (!InitFromConfig(EntityComponent_->GetEntityId())) {
		UE_LOG(LogTemp, Warning, TEXT("通过EntityId进行初始化失败(TowerBase.cpp)")); return;
	}
}

bool ATowerBase::InitFromConfig(FName Id) {
	// ~~读取主表
	if (!EntityBaseTable_) {
		UE_LOG(LogTemp, Warning, TEXT("EntityBaseTable为空(TowerBase.cpp)"));
		return false;
	}

	// ~~通过EntityId找到对应行(Raw)
	const FEntityBaseConfig* BaseConfig =
		EntityBaseTable_->FindRow<FEntityBaseConfig>(Id, TEXT("无法找到对应Raw,(TowerBase.cpp)"));
	if (!BaseConfig) {
		UE_LOG(LogTemp, Warning,
			TEXT("无法找到对应EntityBase配置数据, EntityId = %s(TowerBase.cpp)"), *Id.ToString()); return false;
	}

	// ~~确认Entity种类正确
	if (BaseConfig->EntityType != EEntityType::EntityTower) {
		UE_LOG(LogTemp, Warning, TEXT(" EntityType不匹配,EntityId = %s(TowerBase.cpp)"), *Id.ToString()); return false;
	}

	// ~~Entity组件初始化
	if (!EntityComponent_->InitFromConfig(BaseConfig)) {
		UE_LOG(LogTemp, Warning, TEXT("Entity组件初始化失败(TowerBase.cpp)")); return false;
	}

	// ~~读取Tower扩展表(Tower)
	if (!TowerExtraTable_) {
		UE_LOG(LogTemp, Warning, TEXT("TowerExtraTable为空(TowerBase.cpp)")); return false;
	}

	// ~~通过EntityId找到对应行(Raw)
	const FEntityTowerExtraConfig* TowerConfig =
		TowerExtraTable_->FindRow<FEntityTowerExtraConfig>(Id, TEXT("Lookup TowerExtra"));
	if (!TowerConfig) {
		UE_LOG(LogTemp, Warning,
			TEXT("无法找到对应Tower配置数据, EntityId = %s(TowerBase.cpp)"), *Id.ToString()); return false;
	}

	// ~~Tower字段初始化
	if (!InitTowerFromConfig(TowerConfig)) {
		UE_LOG(LogTemp, Warning, TEXT("Tower扩展字段初始化失败(TowerBase.cpp)")); return false;
	}

	return true;
}

bool ATowerBase::InitTowerFromConfig(const FEntityTowerExtraConfig* TowerConfig) {
	if (!TowerConfig) { return false; }

	Damage_ = TowerConfig->Damage;
	BuildCost_ = TowerConfig->BuildCost;
	return true;
}