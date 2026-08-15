// Fill out your copyright notice in the Description page of Project Settings.

#include "Prinsi/AppSystem/EnemySpawn/AppEnemySpawner.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "Prinsi/Entity/Character/Enemy/AppEnemyCharacterBase.h"
#include "Components/ArrowComponent.h"


// Sets default values
AAppEnemySpawner::AAppEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));

	SetRootComponent(ArrowComp);
}

// Called when the game starts or when spawned
void AAppEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	StartSpawn();	// 开始敌人生成
}

// Called every frame
void AAppEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
* @brief　	敌人生成总开关
* @note		重置后开始第一波
*/
void AAppEnemySpawner::StartSpawn()
{
	if (SpawnWaves.IsEmpty())
	{
		return;
	}

	CurrentWaveIndex = 0;
	CurrentGroupIndex = 0;
	CurrentSpawnCount = 0;

	StartCurrentWave();
}

/**
* @brief　	开始当前敌人刷新波
* @note		重置后开始第一组
*/
void AAppEnemySpawner::StartCurrentWave()
{
	if (!SpawnWaves.IsValidIndex(CurrentWaveIndex))
	{
		return;
	}

	CurrentGroupIndex = 0;

	StartCurrentSpawnGroup();
}

/**
* @brief　	开始当前敌人刷新组
* @note		重置后开始第一个敌人
*/
void AAppEnemySpawner::StartCurrentSpawnGroup()
{
	if (!SpawnWaves.IsValidIndex(CurrentWaveIndex))
	{
		return;
	}
	FAppEnemyWave& CurrentWave = SpawnWaves[CurrentWaveIndex];

	if (!CurrentWave.SpawnGroups.IsValidIndex(CurrentGroupIndex))
	{
		FinishCurrentWave();
		return;
	}

	CurrentSpawnCount = 0;

	SpawnCurrentEnemy();
}

/**
* @brief　	单位敌人刷新
* @note		使用Timer实现自我回调
*/
void AAppEnemySpawner::SpawnCurrentEnemy()
{
	if (!SpawnWaves.IsValidIndex(CurrentWaveIndex))
	{
		return;
	}
	FAppEnemyWave& CurrentWave = SpawnWaves[CurrentWaveIndex];

	if (!CurrentWave.SpawnGroups.IsValidIndex(CurrentGroupIndex))
	{
		FinishCurrentWave();
		return;
	}
	FAppEnemySpawnGroup& CurrentGroup = CurrentWave.SpawnGroups[CurrentGroupIndex];

	// 敌人单位生成
	SpawnEnemy(CurrentGroup.EnemyClass, CurrentGroup.MarchRoute);
	++CurrentSpawnCount;

	// 生成组达到生成上线后结束生成
	if (CurrentSpawnCount >= CurrentGroup.SpawnCount)
	{
		FinishCurrentSpawnGroup();
		return;
	}

	// 设置Tiemr进行自我回调
	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AAppEnemySpawner::SpawnCurrentEnemy,
		CurrentGroup.SpawnInterval,
		false
	);
}

/**
* @brief　	当前敌人刷新组结束
* @note		开启下一刷新组
*/
void AAppEnemySpawner::FinishCurrentSpawnGroup()
{
	++CurrentGroupIndex;

	StartCurrentSpawnGroup();
}

/**
* @brief　	当前敌人刷新波结束
* @note		开启下一刷新波（使用Timer调用StartCurrentWave）
*/
void AAppEnemySpawner::FinishCurrentWave()
{
	if (!SpawnWaves.IsValidIndex(CurrentWaveIndex))
	{
		return;
	}
	const FAppEnemyWave& FinishedWave = SpawnWaves[CurrentWaveIndex];

	++CurrentWaveIndex;
	if (!SpawnWaves.IsValidIndex(CurrentWaveIndex))
	{
		return;		// 全部波次生成结束
	}

	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&AAppEnemySpawner::StartCurrentWave,
		FinishedWave.NextWaveInterval,
		false
	);
}

/**
* @brief　	敌人刷新处理
* @param	InEnemyClass 敌人类型
* @param	InMarchRoute 设置行军路线
*/
AAppEnemyCharacterBase* AAppEnemySpawner::SpawnEnemy(TSubclassOf<AAppEnemyCharacterBase>InEnemyClass, AAppMarchRoute* InMarchRoute)
{
	if (!InEnemyClass || !InMarchRoute)
	{
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	// @todo 先在箭头位置生成
	const FTransform SpawnTransform = ArrowComp->GetComponentTransform();

	// 敌人生成
	AAppEnemyCharacterBase* SpawnedEnemy = World->SpawnActorDeferred<AAppEnemyCharacterBase>(
		InEnemyClass,
		SpawnTransform,
		this,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	SpawnedEnemy->SetMarchRoute(InMarchRoute);
	SpawnedEnemy->FinishSpawning(SpawnTransform);

	return SpawnedEnemy;
}

