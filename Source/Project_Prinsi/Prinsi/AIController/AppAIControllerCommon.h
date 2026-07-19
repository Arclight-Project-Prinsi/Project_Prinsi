// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h" 
#include "AppAIControllerCommon.generated.h"

class UEnvQuery;			// EQS
class AAppTowerBase;
class UStateTreeAIComponent;
class UStateTree;

// @note enum class?
// @note 用于表示角色当前的移动状态
UENUM()
enum class EAppMoveStatus :uint8
{
	Idle,
	Query,
	Moving,
	Succeded,
	Failed,
};


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

protected:
	// @todo 是否需要感知组件?
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	//TObjectPtr<UAIPerceptionComponent>PerceptionComp;	// Component_AI感知组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStateTreeAIComponent>StateTreeComp;		// Component_状态树组件

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Config")
	TObjectPtr<UStateTree>StateTree;			// 状态树

	//ws1----------------------------------

	//――――――――――――――――――――
	// EQS关联
	//――――――――――――――――――――
public:
	// EQS调查（Tower目标）
	bool RequestMoveToAssaultPointTower();		

protected:
	// EQS调查完成回调 & 确认是否调用MoveTo位移
	void OnAssultPointQueryFinished(TSharedPtr<FEnvQueryResult>Result);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI|Config|EQS")
	TObjectPtr<UEnvQuery>FindAssaultPointQuery;	// EQS

	//ws6------------------------------
	//――――――――――――――――――――
	// STT移动关联
	//――――――――――――――――――――
protected:
	// 移动处理完成回调（AIController中绑定）
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)override;

public:
	EAppMoveStatus MoveStatus = EAppMoveStatus::Idle;	// 移动状态（STT根据这个变量运行）

protected:
	bool bWaitingForAssaultMove = false;				// 确保STT专用的MoveStatus不被STT无关的移动所修改

	UPROPERTY(EditDefaultsOnly, Category = "AI|Config|Move")
	float AcceptanceRadius = 80.0f;						// 移动完成的检测距离

	//ws7---------------------------------
public:
	void AbortAssaultMove();

	//ws8---------------------------------
public:
	UPROPERTY()
	TObjectPtr<AAppTowerBase>TargetTower = nullptr;		// @note EQS

};
