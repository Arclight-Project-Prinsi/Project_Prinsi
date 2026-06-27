#pragma once
#include "CoreMinimal.h"
#include "Prinsi\AIController\StateTree\Task\AppST_Task_NonPlayerBase.h"
#include "AppST_Task_Dead.generated.h"

// @todo
// PrinsiStateTreeTaskBase.generated

class AAIController;
class APawn;


// @note â¬à»íËêßèäé˘ìIè„â∫ï∂?
USTRUCT()
struct PROJECT_PRINSI_API FAppST_Task_Dead : public FAppST_Task_NonPlayerBase
{
	GENERATED_BODY()

	// @note â¬à»íËêßèäé˘ìIè„â∫ï∂?
	using FInstanceDataType = FAppNonPlayerStateTreeTaskBaseInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

protected:
	virtual EStateTreeRunStatus OnEnterState(
		FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition,
		FInstanceDataType& InstanceData) const;

	virtual EStateTreeRunStatus OnTick(
		FStateTreeExecutionContext& Context,
		const float DeltaTime,
		FInstanceDataType& InstanceData) const;

	virtual void OnExitState(
		FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition,
		FInstanceDataType& InstanceData) const;
};