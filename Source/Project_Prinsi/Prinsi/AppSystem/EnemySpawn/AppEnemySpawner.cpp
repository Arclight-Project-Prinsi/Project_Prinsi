// Fill out your copyright notice in the Description page of Project Settings.

#include "Prinsi/AppSystem/EnemySpawn/AppEnemySpawner.h"
#include "Prinsi/Define/AppDefineDebug.h"
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

	// @todo 先生成一个敌人测试
	SpawnEnemy();
}

// Called every frame
void AAppEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
* @brief　	敌人生成函数
*/
AAppEnemyCharacterBase* AAppEnemySpawner::SpawnEnemy()
{
	if (!EnemyClass)
	{
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	// @note 生成时若发生重叠
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;;
	// @todo 先在箭头位置生成
	const FTransform SpawnTransform = ArrowComp->GetComponentTransform();

	// ~~敌人生成
	AAppEnemyCharacterBase* SpawnedEnemy = World->SpawnActor<AAppEnemyCharacterBase>(
		EnemyClass.Get(),
		SpawnTransform.GetLocation(),
		SpawnTransform.Rotator(),
		SpawnParams
	);

	if (!SpawnedEnemy)
	{
		return nullptr;
	}

	return SpawnedEnemy;
}

