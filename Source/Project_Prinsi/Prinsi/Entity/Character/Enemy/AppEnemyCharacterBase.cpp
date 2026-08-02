// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/Entity/Character/Enemy/AppEnemyCharacterBase.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "Prinsi/AppSystem/GameLoop/AppMarchRoute.h"	// @scaff
#include "Prinsi/Entity/Tower/AppTowerBase.h"			// @scaff
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

bool AAppEnemyCharacterBase::IsBlocked() const
{
	return IsValid(Blocker);
}

bool AAppEnemyCharacterBase::SetBlocker(AAppTowerBase* InBlocker)
{
	if (!IsValid(InBlocker))
	{
		return false;
	}

	// 如果已经存在阻挡者
	if (IsValid(Blocker) && Blocker != InBlocker)
	{
		return false;
	}

	Blocker = InBlocker;
	return true;

	// @scaff 停止移动处理不应该在设置阻挡中进行
	// 这里只是立即制动。
	// StateTree 随后会从 March 转入 AttackBlocker。
	//if (AAIController* AIC =
	//	Cast<AAIController>(GetController()))
	//{
	//	AIC->StopMovement();
	//}
	//return true;
}

void AAppEnemyCharacterBase::ClearBlocker(AAppTowerBase* InBlocker /*=nullptr*/)
{
	if (InBlocker && Blocker != InBlocker)
	{
		return;
	}

	Blocker = nullptr;
}

AAppTowerBase* AAppEnemyCharacterBase::GetBlocker() const
{
	return IsValid(Blocker) ? Blocker : nullptr;
}
