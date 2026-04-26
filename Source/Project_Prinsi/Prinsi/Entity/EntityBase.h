// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntityBase.generated.h"

UCLASS()
class PROJECT_PRINSI_API AEntityBase : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	//ws------------------------------------
protected:
	virtual void InitFromConfig() {}	// 读取配置表实现初始化

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity")
	FName EntityId;						// 实体ID	

	//ws2------------------------------------
public:
	// Sets default values for this actor's properties
	AEntityBase();



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
