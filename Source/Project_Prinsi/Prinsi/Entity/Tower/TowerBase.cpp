// Fill out your copyright notice in the Description page of Project Settings.


#include "Prinsi/Entity/Tower/TowerBase.h"
#include "Prinsi/Entity/EntityConfigStructs.h"

void ATowerBase::BeginPlay()
{
	Super::BeginPlay();

	//* 读取主表 **//
	if (!EntityBaseTable) {
		UE_LOG(LogTemp, Warning, TEXT("EntityBaseTable为空(TowerBase.cpp)"));
		return;
	}

	const FEntityBaseConfig* BaseConfig =
		EntityBaseTable->FindRow<FEntityBaseConfig>(EntityId, TEXT("Lookup EntityBase"));		//[p]这个Text参数是？

	if (!BaseConfig) {
		UE_LOG(LogTemp, Warning, TEXT("BaseConfig为空,EntityId = %s(TowerBase.cpp)"), *EntityId.ToString());
		return;
	}

	if (BaseConfig->EEntityType != EEntityType::Tower) {
		UE_LOG(LogTemp, Warning, TEXT("EntityType错配,EntityId = %s(TowerBase.cpp)"), *EntityId.ToString());
		return;
	}


	//* 读取Tower扩展表 **//
	if (!TowerExtraTable) {
		UE_LOG(LogTemp, Warning, TEXT("TowerExtraTable为空(TowerBase.cpp)"));
		return;
	}

	const FEntityTowerExtraConfig* TowerConfig =
		TowerExtraTable->FindRow<FEntityTowerExtraConfig>(EntityId, TEXT("Lookup TowerExtra"));		//[p]这个Text参数是？

	if (!TowerConfig) {
		UE_LOG(LogTemp, Warning, TEXT("Tower为空,EntityId = %s(TowerBase.cpp)"), *EntityId.ToString());
		return;
	}

	//[P]读表通过后开始初始化
	InitFromConfig();


	//[P]
	//if (!TowerExtraTable) {
	//	UE_LOG(LogTemp, Warning, TEXT("TowerExtraTable为空(TowerBase.cpp)"));
	//	return;
	//}
}

void ATowerBase::InitFromConfig()
{
	//[p]
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("开始读表初始化(TowerBase.cpp)！")); }
}
