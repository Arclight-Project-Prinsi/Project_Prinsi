// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EntityConfigStructs.h"		// Entity配置文件

#include "EntityBase.generated.h"

UCLASS()
class PROJECT_PRINSI_API AEntityBase : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void InitFromConfig(const FEntityBaseConfig* baseConfig) {}	// 读取配置表实现初始化

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity")
	FName EntityId_;				// 实体ID	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config Entity")
	EEntityFaction Faction_ = EEntityFaction::None;		// 阵营	

	//ws2------------------------------------
public:
	// Sets default values for this actor's properties
	AEntityBase();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
