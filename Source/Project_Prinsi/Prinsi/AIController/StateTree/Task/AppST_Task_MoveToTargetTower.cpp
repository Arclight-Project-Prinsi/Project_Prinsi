#include "Prinsi/AIController/StateTree/Task/AppST_Task_MoveToTargetTower.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "StateTreeExecutionContext.h"		// Need_FStateTreeExecutionContext_STT上下文内容
#include "Prinsi/AIController/AppAIControllerCommon.h"


EStateTreeRunStatus FAppST_Task_MoveToTargetTower::OnEnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const
{
	AAppAIControllerCommon* AppController = Cast<AAppAIControllerCommon>(InstanceData.AIController);
	if (!AppController)
	{
		return EStateTreeRunStatus::Failed;
	}

	// 对Tower目标进行EQS调查
	const bool bRequestSucceeded = AppController->RequestMoveToAssaultPointTower();
	return bRequestSucceeded ?
		EStateTreeRunStatus::Running : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FAppST_Task_MoveToTargetTower::OnTick(FStateTreeExecutionContext& Context, const float DeltaTime, FInstanceDataType& InstanceData) const
{
	AAppAIControllerCommon* AppController = Cast<AAppAIControllerCommon>(InstanceData.AIController);
	if (!AppController)
	{
		return EStateTreeRunStatus::Failed;
	}

	// @memo STT最好不要承接太复杂的业务。
	// @memo MoveTo这个行为实际上由AIController驱动，STT只负责判断结果并驱动状态。
	switch (AppController->MoveStatus)
	{
		// Case_移动完成
	case EAppMoveStatus::Succeded:
		return EStateTreeRunStatus::Succeeded;

		// Case_移动失败
	case EAppMoveStatus::Failed:
		return EStateTreeRunStatus::Failed;

		// Case_移动继续
	case EAppMoveStatus::Idle:
	case EAppMoveStatus::Query:
	case EAppMoveStatus::Moving:
	default:
		return EStateTreeRunStatus::Running;
	}
}

void FAppST_Task_MoveToTargetTower::OnExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const
{
	AAppAIControllerCommon* AppController = Cast<AAppAIControllerCommon>(InstanceData.AIController);
	if (!AppController)
	{
		return;
	}

	// 停止移动
	AppController->AbortAssaultMove();
}