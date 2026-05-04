// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/Entity/Tower/TowerBase.h"


ATowerBase::ATowerBase() {
	PrimaryActorTick.bCanEverTick = true;

	EntityComponent_ = CreateDefaultSubobject<UEntityComponent>(TEXT("EntityComponent"));
}

void ATowerBase::BeginPlay() {
	Super::BeginPlay();

	// ~~获取Entity的id
	if (!EntityComponent_) {
		UE_LOG(LogTemp, Warning, TEXT("EntityComponent为空(TowerBase.cpp)")); return;
	}
	const FName EntityId = EntityComponent_->GetEntityId();
	if (EntityId.IsNone()) {
		UE_LOG(LogTemp, Warning, TEXT("EntityId为空(TowerBase.cpp)")); return;
	}


	// ~~读取主表
	if (!EntityBaseTable_) {
		UE_LOG(LogTemp, Warning, TEXT("EntityBaseTable为空(TowerBase.cpp)"));
		return;
	}

	// ~~通过EntityId找到对应行(Raw)
	const FEntityBaseConfig* BaseConfig =
		EntityBaseTable_->FindRow<FEntityBaseConfig>(EntityId, TEXT("无法找到对应Raw,(TowerBase.cpp)"));
	if (!BaseConfig) {
		UE_LOG(LogTemp, Warning,
			TEXT("无法找到对应EntityBase配置数据, EntityId = %s(TowerBase.cpp)"), *EntityId.ToString()); return;
	}

	// ~~确认Entity种类正确
	if (BaseConfig->EntityType != EEntityType::EntityTower) {
		UE_LOG(LogTemp, Warning, TEXT(" EntityType不匹配,EntityId = %s(TowerBase.cpp)"), *EntityId.ToString());
		return;
	}

	// ~~Entity组件初始化
	EntityComponent_->InitFromConfig(BaseConfig);

	// ~~读取Tower扩展表(Tower)
	if (!TowerExtraTable_) {
		UE_LOG(LogTemp, Warning, TEXT("TowerExtraTable为空(TowerBase.cpp)")); return;
	}

	// ~~通过EntityId找到对应行(Raw)
	const FEntityTowerExtraConfig* TowerConfig =
		TowerExtraTable_->FindRow<FEntityTowerExtraConfig>(EntityId, TEXT("Lookup TowerExtra"));
	if (!TowerConfig) {
		UE_LOG(LogTemp, Warning,
			TEXT("无法找到对应Tower配置数据, EntityId = %s(TowerBase.cpp)"), *EntityId.ToString());
		return;
	}

	// ~~Tower字段初始化
	InitTowerFromConfig(TowerConfig);	// Tower初始化
}

void ATowerBase::InitTowerFromConfig(const FEntityTowerExtraConfig* TowerConfig) {
	Damage_ = TowerConfig->Damage;
	BuildCost_ = TowerConfig->BuildCost;
}