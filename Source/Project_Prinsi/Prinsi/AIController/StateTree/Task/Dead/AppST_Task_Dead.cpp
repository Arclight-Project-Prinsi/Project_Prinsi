#include "Prinsi/AIController/StateTree/Task/Dead/AppST_Task_Dead.h"
#include "StateTreeExecutionContext.h"		// Need_FStateTreeExecutionContext_STTè„â∫ï∂ì‡óe
#include "Prinsi/Define/AppDefineDebug.h"
#include "AIController.h"


// @note
EStateTreeRunStatus FAppST_Task_Dead::EnterState(
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

EStateTreeRunStatus FAppST_Task_Dead::Tick(
	FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	// @todo
	APP_SCR_ERROR(TEXT("Dead"));

	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	return OnTick(Context, DeltaTime, InstanceData);
}

void FAppST_Task_Dead::ExitState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	OnExitState(Context, Transition, InstanceData);
}

EStateTreeRunStatus FAppST_Task_Dead::OnEnterState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition,
	FInstanceDataType& InstanceData) const
{
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FAppST_Task_Dead::OnTick(
	FStateTreeExecutionContext& Context,
	const float DeltaTime,
	FInstanceDataType& InstanceData) const
{
	return EStateTreeRunStatus::Running;
}

void FAppST_Task_Dead::OnExitState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition,
	FInstanceDataType& InstanceData) const
{
}