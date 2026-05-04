// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/Component/EntityComponent.h"

// Sets default values for this component's properties
UEntityComponent::UEntityComponent() {
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEntityComponent::BeginPlay() {
	Super::BeginPlay();

	// ...
}


// Called every frame
void UEntityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEntityComponent::InitFromConfig(const FEntityBaseConfig* BaseConfig) {
	if (!BaseConfig) {
		UE_LOG(LogTemp, Warning, TEXT("EntityComponent初始化失败：BaseConfig为空(EntityComponent.cpp)"));
		return;
	}

	// ~~字段初始化
	EntityId_ = BaseConfig->EntityId;		// Id
	EntityType_ = BaseConfig->EntityType;	// 类型
	Faction_ = BaseConfig->EntityFaction;	// 阵营
	//[D]
	UE_LOG(LogTemp, Log, TEXT("EntityComponent初始化成功：EntityId = %s"), *EntityId_.ToString());
}

