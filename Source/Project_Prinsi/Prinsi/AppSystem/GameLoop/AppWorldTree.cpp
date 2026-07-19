// Fill out your copyright notice in the Description page of Project Settings.

#include "Prinsi/AppSystem/GameLoop/AppWorldTree.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "Prinsi/Entity/Character/Enemy/AppEnemyCharacterBase.h"
#include "Components/BoxComponent.h"


// Sets default values
AAppWorldTree::AAppWorldTree()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	CollisionArrivalComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	SetRootComponent(RootComp);
	StaticMeshComp->SetupAttachment(RootComp);
	CollisionArrivalComp->SetupAttachment(RootComp);

	// ~~InitComp_StaticMesh
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ~~InitComp_CollisionArrival
	CollisionArrivalComp->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
	CollisionArrivalComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionArrivalComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionArrivalComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionArrivalComp->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AAppWorldTree::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionArrivalComp)
	{
		// @memo AddUniqueDynamic_防御措施。同一对象的同一函数，只会绑定一次。
		CollisionArrivalComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &AAppWorldTree::OnCollisionArrivalBeginOverlap);
	}
}

// Called every frame
void AAppWorldTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
* @brief　	碰撞盒判定
* @memo　	与敌人（AppEnemyCharacterBase）接触后调用碰撞处理。
*/
void AAppWorldTree::OnCollisionArrivalBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAppEnemyCharacterBase* Enemy = Cast<AAppEnemyCharacterBase>(OtherActor);

	if (!Enemy)
	{
		return;
	}

	HandleEnemyArrived(Enemy);
}

void AAppWorldTree::HandleEnemyArrived(AAppEnemyCharacterBase* Enemy)
{
	if (!Enemy)
	{
		return;
	}

	// @todo 这里可以给Enemy一个已抵达GameplayTag，避免重复结算

	Enemy->ArrivedDestination();

	if (Enemy->TeleportTo(EnemyDestination, FRotator::ZeroRotator, false, true))
	{
		// @todo
		APP_SCR_ERROR(TEXT("敌人抵达终点"));
	}
}

