#pragma once
#include "CoreMinimal.h"
#include "Prinsi\AIController\StateTree\Task\AppST_Task_NonPlayerBase.h"
#include "AppST_Task_AcquireTarget.generated.h"

class AAIController;
class APawn;


USTRUCT()
struct PROJECT_PRINSI_API FAppST_Task_AcquireTarget : public FAppST_Task_NonPlayerBase
{
	GENERATED_BODY()

	// @todo
	//using FInstanceDataType = FAppNonPlayerStateTreeTaskBaseInstanceData;
	
	// @todo
	// ~~STT执行用数据副本
//public:
//	virtual const UStruct* GetInstanceDataType() const override
//	{
//		return FInstanceDataType::StaticStruct();
//	}


	// ~~STT实际执行部分
public:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

protected:
	// 进入State的处理
	virtual EStateTreeRunStatus OnEnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const;
	// State更新处理
	virtual EStateTreeRunStatus OnTick(FStateTreeExecutionContext& Context, const float DeltaTime, FInstanceDataType& InstanceData) const;
	// 退出State处理
	virtual void OnExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const;
};