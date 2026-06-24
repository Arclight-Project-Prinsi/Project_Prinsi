// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/AIController/AppAIControllerCommon.h"
#include "Prinsi/Define/AppDefineDebug.h"				// Define_Debug工具
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "Navigation/PathFollowingComponent.h"			// @note 移动结果查询用


AAppAIControllerCommon::AAppAIControllerCommon()
{

}

void AAppAIControllerCommon::BeginPlay()
{
	Super::BeginPlay();
}

void AAppAIControllerCommon::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// @todo 测试EQS
	//RunFindAssaultPointQuery();
	MoveToAssualtPoint();
}

void AAppAIControllerCommon::RunFindAssaultPointQuery()
{
	if (!FindAssaultPointQuery)
	{
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance =
		UEnvQueryManager::RunEQSQuery(
			GetWorld(),
			FindAssaultPointQuery,
			GetPawn(),								// @note Querier
			EEnvQueryRunMode::SingleResult,			// @note
			nullptr
		);

	if (!QueryInstance)
	{
		return;
	}

	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AAppAIControllerCommon::OnFindAssaultPointQueryFinished);
}

void AAppAIControllerCommon::OnFindAssaultPointQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (!QueryInstance || QueryStatus != EEnvQueryStatus::Success)
	{
		return;
	}

	// @note 返回查询结果?
	TArray<FVector> ResultLocations;
	QueryInstance->GetQueryResultsAsLocations(ResultLocations);

	// @note 没查询到结果
	if (ResultLocations.IsEmpty())
	{
		return;
	}

	const FVector AssaultPoint = ResultLocations[0];	// @note 获得第一个结果
	MoveToLocation(AssaultPoint, AcceptanceRadius);		// @note 这个与AIMoveTo相同吗?
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

	// @note
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

	CurrentAssualtPoint = AssaultPoint;		// @note 成功测试
	bWaitingForAssaultMove = true;			// @note 成功测试


	const EPathFollowingRequestResult::Type MoveRequestResult =		// @note 成功测试
		MoveToLocation(AssaultPoint, 50.0f);

	// @note 成功测试
	if (MoveRequestResult == EPathFollowingRequestResult::Failed)
	{
		bWaitingForAssaultMove = false;
		OnAssaultMoveFailed(EPathFollowingResult::Invalid);
	}
	else if (MoveRequestResult == EPathFollowingRequestResult::Failed)
	{
		bWaitingForAssaultMove = false;
		OnAssaultMoveSucceeded();
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

	// @note Result本身就有一个Code，根据Code判定MoveTo的处理结果
	if (Result.Code == EPathFollowingResult::Success)
	{
		OnAssaultMoveSucceeded();	// @note 成功
	}
	else
	{
		OnAssaultMoveFailed(Result.Code);	// @note 失败
	}
}

void AAppAIControllerCommon::OnAssaultMoveSucceeded()
{
	APP_SCREEN_ERROR(TEXT("EQS查询后位移成功!"));
}

void AAppAIControllerCommon::OnAssaultMoveFailed(EPathFollowingResult::Type ResultCode)
{
	APP_SCREEN_ERROR(TEXT("EQS查询后位移失败!!"));
}
