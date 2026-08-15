// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/Entity/Tower/AppTowerBase.h"
// Prinsi
#include "Prinsi/Entity/Character/Enemy/AppEnemyCharacterBase.h"
// Misc
#include "Components/BoxComponent.h"
#include "Prinsi/Define/AppDefineDebug.h"


AAppTowerBase::AAppTowerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ~~初始化组件
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	TowerMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	EntityComp = CreateDefaultSubobject<UEntityComponent>(TEXT("Entity"));
	BoxBlockEnemyComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxBlockEnemy"));

	TowerMeshComp->SetupAttachment(RootComponent);
	BoxBlockEnemyComp->SetupAttachment(RootComponent);

	// ~初始化组件_BoxBlockEnemyComp
	{
		BoxBlockEnemyComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxBlockEnemyComp->SetGenerateOverlapEvents(true);
	}
}

void AAppTowerBase::BeginPlay()
{
	Super::BeginPlay();

	//[p]
	// ~~通过EntityId进行初始化(主表+扩展表)
	//if (!EntityComp) {
	//	UE_LOG(LogTemp, Warning, TEXT("Entity组件为空_TowerBase.cpp")); return;
	//}
	//if (!InitFromConfig(EntityComp->GetEntityId())) {
	//	UE_LOG(LogTemp, Warning, TEXT("通过EntityId进行初始化失败(TowerBase.cpp)")); return;
	//}

	BoxBlockEnemyComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &AAppTowerBase::OnBoxBlockEnemyBeginOverlap);
}

bool AAppTowerBase::InitFromConfig(FName Id)
{
	// ~~读取主表(Main)
	if (!EntityBaseTable)
	{
		return false;
	}

	// ~通过EntityId找到对应Raw
	const FEntityBaseConfig* BaseConfig =
		EntityBaseTable->FindRow<FEntityBaseConfig>(Id, TEXT("failed_FindRow_EntityBase"));
	if (!BaseConfig)
	{
		return false;
	}

	// ~确认Entity种类正确
	if (BaseConfig->EntityType != EEntityType::EntityTower)
	{
		return false;
	}

	// ~Entity组件初始化
	if (!EntityComp->InitFromConfig(BaseConfig))
	{
		return false;
	}


	// ~~读取Tower扩展表(Extra)
	if (!TowerExtraTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("TowerExtraTable为空_TowerBase.cpp")); return false;
	}

	// ~通过EntityId找到对应Raw
	const FEntityTowerExtraConfig* TowerConfig = TowerExtraTable->FindRow<FEntityTowerExtraConfig>(Id, TEXT("failed_FindRow_TowerExtra"));
	if (!TowerConfig)
	{
		return false;
	}

	// ~Tower属性初始化
	if (!InitTowerFromConfig(TowerConfig))
	{
		return false;
	}

	return true;
}

bool AAppTowerBase::InitTowerFromConfig(const FEntityTowerExtraConfig* TowerConfig)
{
	if (!TowerConfig) { return false; }

	Damage = TowerConfig->Damage;				// 伤害
	BuildCost = TowerConfig->BuildCost;			// 建造花费
	FootprintSize = TowerConfig->FootprintSize;	// 占地

	return true;
}

bool AAppTowerBase::InitialTower()
{
	if (bIsActive) { return false; }
	if (!EntityComp) { return false; }

	bIsActive = InitFromConfig(EntityComp->GetEntityId());
	return bIsActive;
}

void AAppTowerBase::OnBoxBlockEnemyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAppEnemyCharacterBase* Enemy = Cast<AAppEnemyCharacterBase>(OtherActor);
	if (!Enemy)
	{
		return;
	}

	TryBlockEnemy(Enemy);
}

bool AAppTowerBase::TryBlockEnemy(AAppEnemyCharacterBase* Enemy)
{
	if (!IsValid(Enemy))
	{
		return false;
	}

	if (!CanBlockEnemy(Enemy))
	{
		return false;
	}

	// @sacff TArray的AddUnique
	BlockedEnemies.AddUnique(Enemy);

	if (!Enemy->SetBlocker(this))
	{
		BlockedEnemies.Remove(Enemy);
		return false;
	}

	//@sc
	APP_SCR_ERROR(TEXT("阻挡成功!"));

	return true;
}

void AAppTowerBase::ReleaseEnemy(AAppEnemyCharacterBase* Enemy)
{
	if (!Enemy)
	{
		return;
	}

	BlockedEnemies.Remove(Enemy);

	if (Enemy->GetBlocker() == this)
	{
		Enemy->ClearBlocker(this);
	}
}

bool AAppTowerBase::CanBlockEnemy(const AAppEnemyCharacterBase* Enemy) const
{
	if (!IsValid(Enemy))
	{
		return false;
	}

	if (BlockedEnemies.Num() >= MaxBlockCount)
	{
		return false;
	}

	if (Enemy->IsBlocked())
	{
		return false;
	}

	return true;
}

int32 AAppTowerBase::GetCurrentBlockCount() const
{
	return int32();
}

