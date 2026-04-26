// Fill out your copyright notice in the Description page of Project Settings.


#include "Prinsi/Entity/Tower/TowerBase.h"

#include "Prinsi/Entity/EntityConfigStructs.h"


void ATowerBase::BeginPlay()
{
	Super::BeginPlay();

	//* 读取主表 **//
	if (!EntityBaseTable_) {
		UE_LOG(LogTemp, Warning, TEXT("EntityBaseTable为空(TowerBase.cpp)"));
		return;
	}
	// 通过EntityId找到对应行(Raw)
	const FEntityBaseConfig* BaseConfig =
		EntityBaseTable_->FindRow<FEntityBaseConfig>(EntityId_, TEXT("无法找到对应Raw,(TowerBase.cpp)"));
	// 确认RawName正确
	if (!BaseConfig) {
		UE_LOG(LogTemp, Warning, TEXT("初始化失败, 无法找到对应的EntityBase配置数据, EntityId = %s(TowerBase.cpp)"), *EntityId_.ToString());
		return;
	}
	// 确认Entity种类正确
	if (BaseConfig->EntityType != EEntityType::Tower) {
		UE_LOG(LogTemp, Warning, TEXT("初始化失败, EntityType不匹配,EntityId = %s(TowerBase.cpp)"), *EntityId_.ToString());
		return;
	}


	//* 读取Tower扩展表(Tower) **//
	if (!TowerExtraTable_) {
		UE_LOG(LogTemp, Warning, TEXT("TowerExtraTable为空(TowerBase.cpp)"));
		return;
	}
	// 通过EntityId找到对应行(Raw)
	const FEntityTowerExtraConfig* TowerConfig =
		TowerExtraTable_->FindRow<FEntityTowerExtraConfig>(EntityId_, TEXT("Lookup TowerExtra"));
	// 确认RawName正确
	if (!TowerConfig) {
		UE_LOG(LogTemp, Warning, TEXT("初始化失败, 无法找到对应的Tower配置数据, EntityId = %s(TowerBase.cpp)"), *EntityId_.ToString());
		return;
	}

	InitFromConfig(BaseConfig);			// 共通初始化
	InitTowerFromConfig(TowerConfig);	// Tower初始化
}

void ATowerBase::InitFromConfig(const FEntityBaseConfig* BaseConfig)
{
	//[TODO]在这里加入Entity的初始化
	Faction_ = BaseConfig->EntityFaction;
}

void ATowerBase::InitTowerFromConfig(const FEntityTowerExtraConfig* TowerConfig)
{
	Damage_ = TowerConfig->Damage;
	BuildCost_ = TowerConfig->BuildCost;
}