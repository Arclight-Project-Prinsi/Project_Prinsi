#pragma once
#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "AppST_Task_NonPlayerBase.generated.h"

class AAIController;
class APawn;


USTRUCT()
struct FAppNonPlayerStateTreeTaskBaseInstanceData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Context")
	TObjectPtr<AAIController> AIController = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Context")
	TObjectPtr<APawn> Pawn = nullptr;
};

USTRUCT()
struct PROJECT_PRINSI_API FAppST_Task_NonPlayerBase : public FStateTreeTaskBase
{
	GENERATED_BODY()

	using FInstanceDataType = FAppNonPlayerStateTreeTaskBaseInstanceData;

public:
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

public:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

protected:
	virtual EStateTreeRunStatus OnEnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const;
	virtual EStateTreeRunStatus OnTick(FStateTreeExecutionContext& Context, const float DeltaTime, FInstanceDataType& InstanceData) const;
	virtual void OnExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, FInstanceDataType& InstanceData) const;
};