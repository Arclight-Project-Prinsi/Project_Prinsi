#include "Prinsi/AIController/StateTree/Task/AppST_Task_NonPlayerBase.h"
#include "StateTreeExecutionContext.h"		// Need_FStateTreeExecutionContext_STTè„â∫ï∂ì‡óe
#include "AIController.h"


// @note
EStateTreeRunStatus FAppST_Task_NonPlayerBase::EnterState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	InstanceData.AIController = Cast<AAIController>(Context.GetOwner());

	InstanceData.Pawn = InstanceData.AIController ? InstanceData.AIController->GetPawn() : nullptr;

	if (!InstanceData.AIController || !InstanceData.Pawn)
	{
		return EStateTreeRunStatus::Failed;
	}

	return OnEnterState(Context, Transition, InstanceData);
}

EStateTreeRunStatus FAppST_Task_NonPlayerBase::Tick(
	FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	return OnTick(Context, DeltaTime, InstanceData);
}

void FAppST_Task_NonPlayerBase::ExitState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	OnExitState(Context, Transition, InstanceData);
}

EStateTreeRunStatus FAppST_Task_NonPlayerBase::OnEnterState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition,
	FInstanceDataType& InstanceData) const
{
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FAppST_Task_NonPlayerBase::OnTick(
	FStateTreeExecutionContext& Context,
	const float DeltaTime,
	FInstanceDataType& InstanceData) const
{
	return EStateTreeRunStatus::Running;
}

void FAppST_Task_NonPlayerBase::OnExitState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition,
	FInstanceDataType& InstanceData) const
{
}