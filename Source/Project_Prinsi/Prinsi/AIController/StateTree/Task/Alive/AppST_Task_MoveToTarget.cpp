#include "Prinsi/AIController/StateTree/Task/Alive/AppST_Task_MoveToTarget.h"
//#include "Kismet/GameplayStatics.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "StateTreeExecutionContext.h"		// Need_FStateTreeExecutionContext_STT上下文内容
//#include "AIController.h"			// @todo
#include "Prinsi/AIController/AppAIControllerCommon.h"
/*#include "Prinsi/Entity/Tower/AppTowerBase.h"*/	// Actor_Tower基类


//EStateTreeRunStatus FAppST_Task_MoveToTarget::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
//{
//	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
//	InstanceData.AIController = Cast<AAIController>(Context.GetOwner());
//	InstanceData.Pawn = InstanceData.AIController ? InstanceData.AIController->GetPawn() : nullptr;
//
//	if (!InstanceData.AIController || !InstanceData.Pawn)
//	{
//		return EStateTreeRunStatus::Failed;
//	}
//
//	return OnEnterState(Context, Transition, InstanceData);
//}
//
//EStateTreeRunStatus FAppST_Task_MoveToTarget::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
//{
//	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
//	return OnTick(Context, DeltaTime, InstanceData);
//}
//
//void FAppST_Task_MoveToTarget::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
//{
//	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
//	OnExitState(Context, Transition, InstanceData);
//}

EStateTreeRunStatus FAppST_Task_MoveToTarget::OnEnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const
{
	// @note Check_ACI
	AAppAIControllerCommon* AppController = Cast<AAppAIControllerCommon>(InstanceData.AIController);
	if (!AppController)
	{
		return EStateTreeRunStatus::Failed;
	}

	// @note 确认EQS点位登记成功与否
	const bool bRequestSucceeded = AppController->RequestMoveToAssaultPoint();
	return bRequestSucceeded ?
		EStateTreeRunStatus::Running : EStateTreeRunStatus::Failed;

	//// @todo ~~寻找所有Tower对象
	//TArray<AActor*> FoundTowers;
	//UGameplayStatics::GetAllActorsOfClass(Context.GetWorld(), AAppTowerBase::StaticClass(), FoundTowers);
	//if (FoundTowers.IsEmpty())
	//{
	//	AppController->TargetTower = nullptr;
	//	// @todo 进入状态失败会怎样呢?
	//	return EStateTreeRunStatus::Failed;
	//}

	//// ~~寻找最近Tower对象
	//AAppTowerBase* NearestTower = nullptr;
	//float NearestDisSq = TNumericLimits<float>::Max();
	//// STT执行对象的当前位置
	//const FVector PawnLocation = InstanceData.Pawn->GetActorLocation();
	//for (AActor* Actor : FoundTowers)
	//{
	//	AAppTowerBase* Tower = Cast<AAppTowerBase>(Actor);
	//	if (!Tower)
	//	{
	//		continue;
	//	}

	//	const float DisSq = FVector::DistSquared(PawnLocation, Tower->GetActorLocation());

	//	if (DisSq < NearestDisSq)
	//	{
	//		NearestDisSq = DisSq;
	//		NearestTower = Tower;
	//	}
	//}
	//// ~A. 没找到Tower对象
	//if (!NearestTower)
	//{
	//	AppController->TargetTower = nullptr;
	//	return EStateTreeRunStatus::Failed;
	//}
	//// ~B. 找到最近Tower对象
	//AppController->TargetTower = NearestTower;

	//// @todo
	//APP_SCR_ERROR(TEXT("找到Tower对象辣!!"));

	// @meme EStateTreeRunStatus::Succeeded_Enter执行完成后，会直接尝试调用Exit的处理。
	//return EStateTreeRunStatus::Running;		// @note 瞬间完成的ST不需要Tick?直接返回完成即可?
}

EStateTreeRunStatus FAppST_Task_MoveToTarget::OnTick(FStateTreeExecutionContext& Context, const float DeltaTime, FInstanceDataType& InstanceData) const
{
	// @note Check_ACI
	AAppAIControllerCommon* AppController = Cast<AAppAIControllerCommon>(InstanceData.AIController);
	if (!AppController)
	{
		return EStateTreeRunStatus::Failed;
	}

	// @note 通过MoveStatus驱动STT?
	switch (AppController->MoveStatus)
	{
		// @note Case_移动完成
	case EAppMoveStatus::Succeded:
		return EStateTreeRunStatus::Succeeded;

		// @note Case_移动失败
	case EAppMoveStatus::Failed:
		return EStateTreeRunStatus::Failed;

	case EAppMoveStatus::Idle:
	case EAppMoveStatus::Query:
	case EAppMoveStatus::Moving:
	default:
		return EStateTreeRunStatus::Running;
	}

	// @todo
	//APP_SCR_ERROR(TEXT("Tick测试"));
	//return EStateTreeRunStatus::Running;
}

void FAppST_Task_MoveToTarget::OnExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const
{
	// @note Check_ACI
	AAppAIControllerCommon* AppController = Cast<AAppAIControllerCommon>(InstanceData.AIController);
	if (!AppController)
	{
		return;
	}

	// @todo
	APP_SCR_ERROR(TEXT("抵达目标Tower位置!"));

	// @note
	AppController->AbortAssaultMove();
}