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
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(
		GetWorld(),
		AAppTowerBase::StaticClass()
	);
	TargetTower = Cast<AAppTowerBase>(FoundActor);

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
	if (!Result.IsValid())
	{
		return;
	}

	// @note 这是?
	if (Result->IsAborted())
	{
		return;
	}

	if (!Result->IsSuccessful())
	{
		return;
	}

	FVector AssaultPoint = Result->GetItemAsLocation(0);

	// ~~MoveTo任务执行
	const EPathFollowingRequestResult::Type MoveRequestResult = MoveToLocation(AssaultPoint, 50.0f);
	// @scaff  MoveTo请求成功测试
	if (MoveRequestResult == EPathFollowingRequestResult::Failed)
	{
		APP_SCR_ERROR(TEXT("MoveTo请求失败"));

	}
	else if (MoveRequestResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		APP_SCR_ERROR(TEXT("MoveTo请求失败(已处于终点)"));
		OnAssaultMoveSucceeded();
	}
}

void AAppAIControllerCommon::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	// @scaff  MoveTo请求成功测试
	// @memo MoveTo本身就有一个Reuslt结果
	if (Result.Code == EPathFollowingResult::Success)
	{
		OnAssaultMoveSucceeded();
	}
	else
	{
		OnAssaultMoveFailed(Result.Code);
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
