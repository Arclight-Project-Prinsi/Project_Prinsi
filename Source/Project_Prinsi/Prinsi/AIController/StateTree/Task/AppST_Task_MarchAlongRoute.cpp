#include "Prinsi/AIController/StateTree/Task/AppST_Task_MarchAlongRoute.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "StateTreeExecutionContext.h"		// Need_FStateTreeExecutionContext_STT上下文内容
#include "Prinsi/AIController/AppAIControllerCommon.h"


EStateTreeRunStatus FAppST_Task_MarchAlongRoute::OnEnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const
{
	AAppAIControllerCommon* AppController = Cast<AAppAIControllerCommon>(InstanceData.AIController);
	if (!AppController)
	{
		return EStateTreeRunStatus::Failed;
	}

	// 确认行军移动前置处理结果
	const bool bRequestSucceeded = AppController->RequestMoveAlongMarchRoute();
	return bRequestSucceeded ?
		EStateTreeRunStatus::Running : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FAppST_Task_MarchAlongRoute::OnTick(FStateTreeExecutionContext& Context, const float DeltaTime, FInstanceDataType& InstanceData) const
{
	AAppAIControllerCommon* AppController = Cast<AAppAIControllerCommon>(InstanceData.AIController);
	if (!AppController)
	{
		return EStateTreeRunStatus::Failed;
	}

	switch (AppController->MoveStatus)
	{
	case EAppMoveStatus::Succeded:	// Case_移动完成
	{
		return EStateTreeRunStatus::Succeeded;
	}
	case EAppMoveStatus::Failed:	// Case_移动失败
	{
		return EStateTreeRunStatus::Failed;
	}
	case EAppMoveStatus::Moving:	// Case_移动继续
	{
		return EStateTreeRunStatus::Running;
	}
	default:
	{
		return EStateTreeRunStatus::Running;
	}
	}
}

void FAppST_Task_MarchAlongRoute::OnExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const
{
	// @scaff
	APP_ERROR(TEXT("行军移动STT结束!"));

	AAppAIControllerCommon* AppController = Cast<AAppAIControllerCommon>(InstanceData.AIController);
	if (!AppController)
	{
		return;
	}

	// 停止移动
	AppController->AbortAssaultMove();
}