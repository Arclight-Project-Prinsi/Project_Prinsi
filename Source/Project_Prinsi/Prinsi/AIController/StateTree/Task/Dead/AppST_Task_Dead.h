#pragma once

#include "CoreMinimal.h"
#include "Prinsi\AIController\StateTree\Task\AppST_Task_NonPlayerBase.h"
#include "AppST_Task_Dead.generated.h"

class AAIController;
class APawn;


USTRUCT(meta=(DisplayName = "App Task Dead"))
struct PROJECT_PRINSI_API FAppST_Task_Dead : public FAppST_Task_NonPlayerBase
{
	GENERATED_BODY()

	using FInstanceDataType = FAppNonPlayerStateTreeTaskBaseInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

protected:
	virtual EStateTreeRunStatus OnEnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const;
	virtual EStateTreeRunStatus OnTick(FStateTreeExecutionContext& Context, const float DeltaTime, FInstanceDataType& InstanceData) const;
	virtual void OnExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const;
};