// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Prinsi/Define/EntityConfigStructs.h"
#include "Components/ActorComponent.h"
#include "EntityComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_PRINSI_API UEntityComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEntityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 从主表配置初始化
	bool InitFromConfig(const FEntityBaseConfig* BaseConfig);

public:
	FName GetEntityId() const { return EntityId_; }
	EEntityType GetEntityType() const { return EntityType_; }
	EEntityFaction GetFaction() const { return Faction_; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity")
	FName EntityId_;		// 查表标识

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Entity")
	EEntityType EntityType_ = EEntityType::EntityNone;		// Entity类型

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Entity")
	EEntityFaction Faction_ = EEntityFaction::FactionNone;	// Entity阵营
};
