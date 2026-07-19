// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/AIController/AppAIControllerCommon.h"
#include "Prinsi/Define/AppDefineDebug.h"				// Define_Debug工具
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "Navigation/PathFollowingComponent.h"			// Need_EPathFollowingRequestResult_查询MoveTo请求的成功与否
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

	// ~~状态树组件启动
	if (!StateTreeComp || !StateTree)
	{
		return;
	}
	StateTreeComp->SetStateTree(StateTree);
	StateTreeComp->StartLogic();
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
		bWaitingForAssaultMove = false;
		MoveStatus = EAppMoveStatus::Failed;
	}
	// B. 已经抵达终点
	else if (MoveRequestResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		bWaitingForAssaultMove = false;
		MoveStatus = EAppMoveStatus::Succeded;

		APP_SCR_ERROR(TEXT("MoveTo请求失败(已处于终点)"));
		//OnAssaultMoveSucceeded();
	}
	// @note
	else
	{
		bWaitingForAssaultMove = true;
		MoveStatus = EAppMoveStatus::Moving;
	}
}

void AAppAIControllerCommon::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (!bWaitingForAssaultMove)
	{
		return;
	}
	bWaitingForAssaultMove = false;

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

bool AAppAIControllerCommon::RequestMoveToAssaultPointTower()
{
	if (!FindAssaultPointQuery)
	{
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}

	if (!TargetTower)
	{
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}

	// ~~EQS调查
	MoveStatus = EAppMoveStatus::Query;
	bWaitingForAssaultMove = false;
	// @memo param2_调查主体（即Querier）
	FEnvQueryRequest QueryRequest(FindAssaultPointQuery, ControlledPawn);

	// ~~返回调查结果 & 确认是否调用MoveTo位移
	QueryRequest.Execute(
		EEnvQueryRunMode::SingleResult,
		//EEnvQueryRunMode::RandomBest5Pct,			// @memo EQS查询结果可以是随机点位
		this,
		&AAppAIControllerCommon::OnAssultPointQueryFinished
	);

	// ~~调查完成
	return true;	
}

void AAppAIControllerCommon::AbortAssaultMove()
{
	if (MoveStatus == EAppMoveStatus::Moving)
	{
		// @note 快进到OnMoveCompleted?
		StopMovement();
	}

	bWaitingForAssaultMove = false;
	MoveStatus = EAppMoveStatus::Idle;		// @note EQS移动处理回到初始状态
}

