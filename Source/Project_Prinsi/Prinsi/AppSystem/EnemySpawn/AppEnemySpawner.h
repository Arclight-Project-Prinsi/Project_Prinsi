// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AppEnemySpawner.generated.h"

class UArrowComponent;
class AAppEnemyCharacterBase;


UCLASS()
class PROJECT_PRINSI_API AAppEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAppEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//ws----------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "EnemySpawn|Function")
	AAppEnemyCharacterBase* SpawnEnemy();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemySpawner|Component")
	TObjectPtr<UArrowComponent> ArrowComp;

protected:
	// @todo 暂时先定死一种敌人类型生成
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemySpawner|Config")
	TSubclassOf<AAppEnemyCharacterBase>EnemyClass;


};
