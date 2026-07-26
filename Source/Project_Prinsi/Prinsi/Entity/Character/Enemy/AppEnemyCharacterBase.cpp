// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/Entity/Character/Enemy/AppEnemyCharacterBase.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "Prinsi/AppSystem/GameLoop/AppMarchRoute.h"	// @scaff
#include "AIController.h"


AAppEnemyCharacterBase::AAppEnemyCharacterBase()
{
	// @memo 生成之后立刻启动AIController
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAppEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// @todo 敌人死亡State测试
	/*AddDeadTag();*/
}

void AAppEnemyCharacterBase::ArrivedDestination()
{
	if (AAIController* AIC = Cast<AAIController>(GetController())) 
	{
		AIC->StopMovement();
	}
}

void AAppEnemyCharacterBase::SetMarchRoute(AAppMarchRoute* InMarchRoute)
{
	MarchRoute = InMarchRoute;
}

AAppMarchRoute* AAppEnemyCharacterBase::GetMarchRoute() const
{
	return MarchRoute;
}
