// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntityBase.generated.h"

UCLASS()
class PROJECT_PRINSI_API AEntityBase : public AActor
{
	GENERATED_BODY()
	
	//ws------------------------------------
public:
	virtual void InitFromConfig() {}			// 读取配置表实现初始化

	//ws2------------------------------------
public:	
	// Sets default values for this actor's properties
	AEntityBase();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
