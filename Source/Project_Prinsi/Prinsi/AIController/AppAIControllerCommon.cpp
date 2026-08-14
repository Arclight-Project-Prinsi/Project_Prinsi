// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/AIController/AppAIControllerCommon.h"
#include "Prinsi/Define/AppDefineDebug.h"				// Define_Debug工具
#include "Prinsi/Entity/Character/Enemy/AppEnemyCharacterBase.h"	// @scaff
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "Navigation/PathFollowingComponent.h"			// Need_EPathFollowingRequestResult_查询MoveTo请求的成功与否
#include "Prinsi/AppSystem/GameLoop/AppMarchRoute.h"	// @scaff
// @todo
//#include "Kismet/GameplayStatics.h"
#include "Prinsi/Entity/Tower/AppTowerBase.h"
// @todo
#include "Components/StateTreeAIComponent.h"


AAppAIControllerCommon::AAppAIControllerCommon()
{
	StateTreeComp = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeComp"));

	// 先不自动启动ST
	StateTreeComp->SetStartLogicAutomatically(false);
}

void AAppAIControllerCommon::BeginPlay()
{
	Super::BeginPlay();

}

void AAppAIControllerCommon::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// **ST（状态树）启动前所需初始化
	const AAppEnemyCharacterBase* Enemy = Cast<AAppEnemyCharacterBase>(InPawn);
	if (!Enemy)
	{
		return;
	}

	// *绑定行军路线（MarchRoute）
	MarchRoute = Enemy->GetMarchRoute();
	if (!MarchRoute)
	{
		return;
	}

	// **ST组件启动
	if (!StateTreeComp || !StateTree)
	{
		return;
	}
	StateTreeComp->SetStateTree(StateTree);
	StateTreeComp->StartLogic();
}

/**
* @brief　	开始移动行军前进行路径点确认
*/
bool AAppAIControllerCommon::RequestMoveAlongMarchRoute()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}

	if (!MarchRoute)
	{
		// @thumb
		APP_SCR_ERROR(TEXT("（thumb）AIC沒找到行军路线。"));
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}

	if (MarchRoute->GetRoutePointCount() <= 0)
	{
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}

	CurrentMarchRoutePointIndex = 0;
	bWaitingAssaultMove = false;
	bWaitingMarchMove = false;
	MoveStatus = EAppMoveStatus::Idle;

	return MoveToCurrentMarchRoutePoint();
}

/**
* @brief　	开始移动行军向当前目标路径点
*/
bool AAppAIControllerCommon::MoveToCurrentMarchRoutePoint()
{
	if (!MarchRoute)
	{
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}

	// **移动完成（抵达终点）
	const int32 RoutePointCount = MarchRoute->GetRoutePointCount();
	if (CurrentMarchRoutePointIndex >= RoutePointCount)
	{
		bWaitingMarchMove = false;
		MoveStatus = EAppMoveStatus::Succeded;
		return true;
	}

	// **MoveTo任务执行
	const FVector TargetLocation = MarchRoute->GetRoutePointLocation(CurrentMarchRoutePointIndex);
	const EPathFollowingRequestResult::Type MoveRequestResult = MoveToLocation(TargetLocation, AcceptanceRadiusMarch);
	switch (MoveRequestResult)
	{
	case EPathFollowingRequestResult::RequestSuccessful: // Case_开始移动
	{
		bWaitingMarchMove = true;
		MoveStatus = EAppMoveStatus::Moving;
		return true;
	}
	case EPathFollowingRequestResult::AlreadyAtGoal:	// Case_已处于目标路径点附近（Raius范围内）
	{
		CurrentMarchRoutePointIndex++;
		return MoveToCurrentMarchRoutePoint();			// 开始MoveToLocation去下一个路径点
	}
	case EPathFollowingRequestResult::Failed:			// Case_行军路径点查询失败
	{
		bWaitingMarchMove = false;
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}

	default:
	{
		bWaitingMarchMove = false;
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}
	}
}

void AAppAIControllerCommon::OnAssultPointQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (!Result.IsValid() ||
		Result->IsAborted() ||		// @memo Result->IsAborted_EQS调查被非正常中断
		!Result->IsSuccessful())
	{
		MoveStatus = EAppMoveStatus::Failed;
		return;
	}

	// MoveTo任务执行
	FVector AssaultPoint = Result->GetItemAsLocation(0);
	const EPathFollowingRequestResult::Type MoveRequestResult = MoveToLocation(AssaultPoint, AcceptanceRadius);
	// A. MoveTo请求失败
	if (MoveRequestResult == EPathFollowingRequestResult::Failed)
	{
		bWaitingAssaultMove = false;
		MoveStatus = EAppMoveStatus::Failed;
	}
	// B. 已经抵达终点
	else if (MoveRequestResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		bWaitingAssaultMove = false;
		MoveStatus = EAppMoveStatus::Succeded;

		APP_SCR_ERROR(TEXT("MoveTo请求失败(已处于终点)"));
		//OnAssaultMoveSucceeded();
	}
	// @note
	else
	{
		bWaitingAssaultMove = true;
		MoveStatus = EAppMoveStatus::Moving;
	}
}

void AAppAIControllerCommon::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	// @todo
	// ~~EQS移动结果
	if (bWaitingAssaultMove)
	{
		bWaitingAssaultMove = false;

		// @scaff  MoveTo请求成功测试
		// @memo MoveTo本身就有一个Reuslt结果
		if (Result.Code == EPathFollowingResult::Success)
		{
			MoveStatus = EAppMoveStatus::Succeded;
		}
		else
		{
			MoveStatus = EAppMoveStatus::Failed;
		}
	}

	// **移动行军结果
	if (bWaitingMarchMove)
	{
		bWaitingMarchMove = false;

		// *移动失败
		if (Result.Code != EPathFollowingResult::Success)
		{
			MoveStatus = EAppMoveStatus::Failed;
		}

		CurrentMarchRoutePointIndex++;		// 寻找下一个移动路径点
		MoveToCurrentMarchRoutePoint();		// 移动前往下一个路径点
		return;
	}
}

bool AAppAIControllerCommon::RequestMoveToAssaultPointTower()
{
	if (!FindAssaultPointQuery)
	{
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}

	AAppEnemyCharacterBase* EnemyPawn = Cast<AAppEnemyCharacterBase>(GetPawn());
	if (!EnemyPawn)
	{
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}
	//@SC
	AAppTowerBase* Blocker = EnemyPawn->GetBlocker();
	if (!IsValid(Blocker))
	{
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}
	/*if (!TargetTower)
	{
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}*/
	//APawn* ControlledPawn = GetPawn();
	//if (!ControlledPawn)
	//{
	//	MoveStatus = EAppMoveStatus::Failed;
	//	return false;
	//}

	// EQS调查
	bWaitingAssaultMove = false;
	// @memo param2_调查主体（即Querier）
	FEnvQueryRequest QueryRequest(FindAssaultPointQuery, EnemyPawn);

	// 返回调查结果 & 确认是否调用MoveTo位移
	QueryRequest.Execute(
		EEnvQueryRunMode::SingleResult,
		//EEnvQueryRunMode::RandomBest5Pct,			// @memo EQS查询结果可以是随机点位
		this,
		&AAppAIControllerCommon::OnAssultPointQueryFinished
	);

	//@sc
	APP_SCR_ERROR(TEXT("EQS查询成功，向Blocker移动"))

	// 调查完成
	return true;
}

void AAppAIControllerCommon::AbortAssaultMove()
{
	if (MoveStatus == EAppMoveStatus::Moving)
	{
		//@sc
		APP_SCR_ERROR(TEXT("Blocker移动结束"))

		// @note 快进到OnMoveCompleted?
		StopMovement();
	}

	bWaitingAssaultMove = false;
	bWaitingMarchMove = false;

	MoveStatus = EAppMoveStatus::Idle;		// @note EQS移动处理回到初始状态
}





