#include "Prinsi/AIController/StateTree/Task/Alive/AppST_Task_AcquireTarget.h"
#include "Kismet/GameplayStatics.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "StateTreeExecutionContext.h"		// Need_FStateTreeExecutionContext_STT上下文内容
//#include "AIController.h"			// @todo
#include "Prinsi/AIController/AppAIControllerCommon.h"
#include "Prinsi/Entity/Tower/AppTowerBase.h"	// Actor_Tower基类


EStateTreeRunStatus FAppST_Task_AcquireTarget::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	// @note 确认内容是否有效?
	InstanceData.AIController = Cast<AAIController>(Context.GetOwner());
	InstanceData.Pawn = InstanceData.AIController ? InstanceData.AIController->GetPawn() : nullptr;

	if (!InstanceData.AIController || !InstanceData.Pawn)
	{
		return EStateTreeRunStatus::Failed;
	}

	return OnEnterState(Context, Transition, InstanceData);
}

EStateTreeRunStatus FAppST_Task_AcquireTarget::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	return OnTick(Context, DeltaTime, InstanceData);
}

void FAppST_Task_AcquireTarget::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	OnExitState(Context, Transition, InstanceData);
}

EStateTreeRunStatus FAppST_Task_AcquireTarget::OnEnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const
{
	// @todo
	APP_SCR_ERROR(TEXT("开始查询目标Tower!!"));

	AAppAIControllerCommon* AppController = Cast<AAppAIControllerCommon>(InstanceData.AIController);
	if (!AppController || !InstanceData.Pawn)
	{
		return EStateTreeRunStatus::Failed;
	}

	// @todo ~~寻找所有Tower对象
	TArray<AActor*> FoundTowers;
	UGameplayStatics::GetAllActorsOfClass(Context.GetWorld(), AAppTowerBase::StaticClass(), FoundTowers);
	if (FoundTowers.IsEmpty())
	{
		AppController->TargetTower = nullptr;
		// @todo 进入状态失败会怎样呢?
		return EStateTreeRunStatus::Failed;
	}

	// ~~寻找最近Tower对象
	AAppTowerBase* NearestTower = nullptr;
	float NearestDisSq = TNumericLimits<float>::Max();
	// STT执行对象的当前位置
	const FVector PawnLocation = InstanceData.Pawn->GetActorLocation();
	for (AActor* Actor : FoundTowers)
	{
		AAppTowerBase* Tower = Cast<AAppTowerBase>(Actor);
		if (!Tower)
		{
			continue;
		}

		const float DisSq = FVector::DistSquared(PawnLocation, Tower->GetActorLocation());

		if (DisSq < NearestDisSq)
		{
			NearestDisSq = DisSq;
			NearestTower = Tower;
		}
	}
	// ~A. 没找到Tower对象
	if (!NearestTower)
	{
		AppController->TargetTower = nullptr;
		return EStateTreeRunStatus::Failed;
	}
	// ~B. 找到最近Tower对象
	AppController->TargetTower = NearestTower;

	// @todo
	APP_SCR_ERROR(TEXT("找到Tower对象!!"));

	// @meme EStateTreeRunStatus::Succeeded_Enter执行完成后，会直接尝试调用Exit的处理。
	return EStateTreeRunStatus::Succeeded;		// @note 瞬间完成的ST不需要Tick?直接返回完成即可?
}

EStateTreeRunStatus FAppST_Task_AcquireTarget::OnTick(FStateTreeExecutionContext& Context, const float DeltaTime, FInstanceDataType& InstanceData) const
{
	// @todo
	APP_SCR_ERROR(TEXT("Tick测试"));

	return EStateTreeRunStatus::Running;
}

void FAppST_Task_AcquireTarget::OnExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const
{
	// @todo
	APP_SCR_ERROR(TEXT("Exit测试"));
}