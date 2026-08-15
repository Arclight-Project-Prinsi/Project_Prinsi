// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AppEnemySpawner.generated.h"

class AAppEnemyCharacterBase;
class AAppMarchRoute;
class UArrowComponent;

/**
* @brief　	敌人生成组构造体
* @note		波（Wave）->组（Group）->单位
*/
USTRUCT(BlueprintType)
struct FAppEnemySpawnGroup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAppEnemyCharacterBase> EnemyClass;	// 敌人类型

	UPROPERTY(EditAnywhere)
	TObjectPtr<AAppMarchRoute> MarchRoute;			// 行军路线

	UPROPERTY(EditAnywhere)
	int32 SpawnCount = 1;							// 生成总数

	UPROPERTY(EditAnywhere)
	float SpawnInterval = 1.0f;						// 单位间隔
};

/**
* @brief　	敌人生成波构造体
*/
USTRUCT(BlueprintType)
struct FAppEnemyWave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FAppEnemySpawnGroup> SpawnGroups;		// 波次相关参数

	UPROPERTY(EditAnywhere)
	float NextWaveInterval = 5.0f;					// 波次间隔
};

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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemySpawner|Component")
	TObjectPtr<UArrowComponent> ArrowComp;

	//~~ 敌人生成关联（Spawn） ~~//
public:
	UFUNCTION(BlueprintCallable, Category = "EnemySpawn|Function")
	void StartSpawn();

protected:
	void StartCurrentWave();			// 开始当前敌人刷新波
	void StartCurrentSpawnGroup();		// 开始当前敌人刷新组
	void SpawnCurrentEnemy();			// 生成当前组敌人单位
	void FinishCurrentSpawnGroup();		// 当前敌人刷新组结束
	void FinishCurrentWave();			// 当前敌人刷新波结束

	AAppEnemyCharacterBase* SpawnEnemy(TSubclassOf<AAppEnemyCharacterBase> InEnemyClass, AAppMarchRoute* InMarchRoute);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemySpawner|Config")
	TArray<FAppEnemyWave> SpawnWaves;	// 敌人刷新波设置

private:
	int32 CurrentWaveIndex = 0;
	int32 CurrentGroupIndex = 0;
	int32 CurrentSpawnCount = 0;		// 当前组已生成计数

	FTimerHandle SpawnTimerHandle;
	FTimerHandle WaveTimerHandle;
};
