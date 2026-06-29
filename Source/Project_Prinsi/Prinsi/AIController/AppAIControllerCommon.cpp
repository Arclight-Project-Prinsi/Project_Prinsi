// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/AIController/AppAIControllerCommon.h"
#include "Prinsi/Define/AppDefineDebug.h"				// Define_Debug工具
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "Navigation/PathFollowingComponent.h"			// @note 移动结果查询用
// @todo
#include "Kismet/GameplayStatics.h"
#include "Prinsi/Entity/Tower/AppTowerBase.h"
// @todo
#include "Components/StateTreeAIComponent.h"


AAppAIControllerCommon::AAppAIControllerCommon()
{
	StateTreeComp = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeComp"));
}

void AAppAIControllerCommon::BeginPlay()
{
	Super::BeginPlay();

	if (StateTreeComp && StateTree)
	{
		// @note 状态树组件启动
		StateTreeComp->StartLogic();
	}
}

void AAppAIControllerCommon::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//----------------------------------------------------
	// @todo 先在场景中找Tower对象测试EQS移动
	//AActor* FoundActor = UGameplayStatics::GetActorOfClass(
	//	GetWorld(),
	//	AAppTowerBase::StaticClass()
	//);
	//TargetTower = Cast<AAppTowerBase>(FoundActor);

	//----------------------------------------------------

	// @todo 测试EQS
	//MoveToAssualtPoint();
}

void AAppAIControllerCommon::MoveToAssualtPoint()
{
	if (!FindAssaultPointQuery)
	{
		return;
	}

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	// @memo param2_调查主体（即Querier）
	FEnvQueryRequest QueryRequest(FindAssaultPointQuery, ControlledPawn);

	// @note
	QueryRequest.Execute(
		EEnvQueryRunMode::RandomBest5Pct,			// @note 随机点位
		this,
		&AAppAIControllerCommon::OnAssultPointQueryFinished
	);
}

void AAppAIControllerCommon::OnAssultPointQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (!Result.IsValid() ||
		Result->IsAborted() ||		// @note 这是?
		!Result->IsSuccessful())
	{
		MoveStatus = EAppMoveStatus::Failed;
		return;
	}

	// ~~MoveTo任务执行
	// @todo 先采用第一个点
	FVector AssaultPoint = Result->GetItemAsLocation(0);
	const EPathFollowingRequestResult::Type MoveRequestResult = MoveToLocation(AssaultPoint, AcceptanceRadius);
	// @scaff  MoveTo请求成功测试
	if (MoveRequestResult == EPathFollowingRequestResult::Failed)
	{
		bWaitingForAssaultMove = false;
		MoveStatus = EAppMoveStatus::Failed;
		APP_SCR_ERROR(TEXT("MoveTo请求失败"));

	}
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
		//OnAssaultMoveSucceeded();
	}
	else
	{
		MoveStatus = EAppMoveStatus::Failed;
		//OnAssaultMoveFailed(Result.Code);
	}
}

void AAppAIControllerCommon::OnAssaultMoveSucceeded()
{
	APP_SCR_ERROR(TEXT("EQS查询后位移成功!"));
}

void AAppAIControllerCommon::OnAssaultMoveFailed(EPathFollowingResult::Type ResultCode)
{
	APP_SCR_ERROR(TEXT("EQS查询后位移失败!!"));
}

bool AAppAIControllerCommon::RequestMoveToAssaultPoint()
{
	// @note 先确保EQS的存在
	if (!FindAssaultPointQuery)
	{
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}

	// @note 确保Tower对象的存在
	if (!TargetTower)
	{
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}

	// @note 确保Pawn对象的存在
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		MoveStatus = EAppMoveStatus::Failed;
		return false;
	}

	// @note 进入查询状态
	MoveStatus = EAppMoveStatus::Query;
	bWaitingForAssaultMove = false;

	// @memo param2_调查主体（即Querier）
	FEnvQueryRequest QueryRequest(FindAssaultPointQuery, ControlledPawn);

	// @note
	QueryRequest.Execute(
		EEnvQueryRunMode::RandomBest5Pct,			// @note 随机点位
		this,
		&AAppAIControllerCommon::OnAssultPointQueryFinished
	);

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
