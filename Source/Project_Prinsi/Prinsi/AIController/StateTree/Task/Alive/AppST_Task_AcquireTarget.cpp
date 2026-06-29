#include "Prinsi/AIController/StateTree/Task/Alive/AppST_Task_AcquireTarget.h"
#include "StateTreeExecutionContext.h"		// Need_FStateTreeExecutionContext_STTè„â∫ï∂ì‡óe
#include "Prinsi/Define/AppDefineDebug.h"
#include "AIController.h"


// @note
EStateTreeRunStatus FAppST_Task_AcquireTarget::EnterState(
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

EStateTreeRunStatus FAppST_Task_AcquireTarget::Tick(
	FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	// @todo
	APP_SCR_ERROR(TEXT("or Alive."));

	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	return OnTick(Context, DeltaTime, InstanceData);
}

void FAppST_Task_AcquireTarget::ExitState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	OnExitState(Context, Transition, InstanceData);
}

EStateTreeRunStatus FAppST_Task_AcquireTarget::OnEnterState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition,
	FInstanceDataType& InstanceData) const
{
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FAppST_Task_AcquireTarget::OnTick(
	FStateTreeExecutionContext& Context,
	const float DeltaTime,
	FInstanceDataType& InstanceData) const
{
	return EStateTreeRunStatus::Running;
}

void FAppST_Task_AcquireTarget::OnExitState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition,
	FInstanceDataType& InstanceData) const
{
}