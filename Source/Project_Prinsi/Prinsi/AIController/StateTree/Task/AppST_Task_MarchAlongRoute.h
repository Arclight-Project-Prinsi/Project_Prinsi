#pragma once

#include "CoreMinimal.h"
#include "Prinsi\AIController\StateTree\Task\AppST_Task_NonPlayerBase.h"
#include "AppST_Task_MarchAlongRoute.generated.h"


USTRUCT(meta = (DisplayName = "App Task March Along Route"))
struct PROJECT_PRINSI_API FAppST_Task_MarchAlongRoute : public FAppST_Task_NonPlayerBase
{
	GENERATED_BODY()

protected:
	virtual EStateTreeRunStatus OnEnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const override;
	virtual EStateTreeRunStatus OnTick(FStateTreeExecutionContext& Context, const float DeltaTime, FInstanceDataType& InstanceData) const override;
	virtual void OnExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const override;
};