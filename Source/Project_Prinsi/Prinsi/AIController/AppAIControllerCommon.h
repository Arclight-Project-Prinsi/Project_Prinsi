// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h" 
#include "AppAIControllerCommon.generated.h"

// @todo
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;


/**
 *
 */
UCLASS()
class PROJECT_PRINSI_API AAppAIControllerCommon : public AAIController
{
	GENERATED_BODY()

public:
	AAppAIControllerCommon();

protected:
	virtual void BeginPlay()override;
	void OnPossess(APawn* InPawn)override;

	//ws---------------------------------------
public:
	// @note "找点"规则，并非点本身
	UFUNCTION(BlueprintCallable)
	void RunFindAssaultPointQuery();		// @note 启动时调用

private:
	UFUNCTION()
	void OnFindAssaultPointQueryFinished(
		UEnvQueryInstanceBlueprintWrapper* QueryInstance,
		EEnvQueryStatus::Type QueryStatus
	);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI|Config|EQS")
	TObjectPtr<UEnvQuery>FindAssaultPointQuery;				// @note EQS

	UPROPERTY(EditDefaultsOnly, Category = "AI|Config|Move")
	float AcceptanceRadius = 80.0f;	// @note 检测距离?

	//ws2--------------------------------------
protected:
	void MoveToAssualtPoint();

	void OnAssultPointQueryFinished(TSharedPtr<FEnvQueryResult>Result);

	//ws3--------------------------------
	// @note 测试用
protected:
	// @note AIRequestID
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)override;
	void OnAssaultMoveSucceeded();
	void OnAssaultMoveFailed(EPathFollowingResult::Type ResultCode);

protected:
	bool bWaitingForAssaultMove = false;		// @note
	FVector CurrentAssualtPoint = FVector::ZeroVector;
};
