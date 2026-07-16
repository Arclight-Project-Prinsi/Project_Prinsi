#include "Prinsi/AIController/StateTree/Task/Dead/AppST_Task_Dead.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "StateTreeExecutionContext.h"		
#include "AIController.h"


EStateTreeRunStatus FAppST_Task_Dead::OnEnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const
{
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FAppST_Task_Dead::OnTick(FStateTreeExecutionContext& Context, const float DeltaTime, FInstanceDataType& InstanceData) const
{
	return EStateTreeRunStatus::Running;
}

void FAppST_Task_Dead::OnExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const
{

}