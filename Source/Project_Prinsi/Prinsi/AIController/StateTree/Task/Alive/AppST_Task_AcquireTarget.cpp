#include "Prinsi/AIController/StateTree/Task/Alive/AppST_Task_AcquireTarget.h"
#include "Kismet/GameplayStatics.h"				// Need_UGameplayStatics::GetAllActorsOfClass
#include "Prinsi/Define/AppDefineDebug.h"		// @scaff Debug工具
#include "StateTreeExecutionContext.h"			// Need_FStateTreeExecutionContext_STT上下文内容
#include "Prinsi/AIController/AppAIControllerCommon.h"
#include "Prinsi/Entity/Tower/AppTowerBase.h"	// Actor_Tower基类


EStateTreeRunStatus FAppST_Task_AcquireTarget::OnEnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const
{
	// @todo STT不适合承接太复杂的业务，距离计算尚可，如果之后存在诸如“根据仇恨值选定目标”等，最好外包出去。
	AAppAIControllerCommon* AppController = Cast<AAppAIControllerCommon>(InstanceData.AIController);
	if (!AppController || !InstanceData.Pawn)
	{
		return EStateTreeRunStatus::Failed;
	}

	// ~~寻找所有Tower对象
	TArray<AActor*> FoundTowers;
	UGameplayStatics::GetAllActorsOfClass(Context.GetWorld(), AAppTowerBase::StaticClass(), FoundTowers);
	if (FoundTowers.IsEmpty())
	{
		AppController->TargetTower = nullptr;
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


	// @memo EStateTreeRunStatus::Succeeded_Enter执行完成后，会直接尝试调用Exit的处理。
	return EStateTreeRunStatus::Succeeded;
}

void FAppST_Task_AcquireTarget::OnExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const
{

}