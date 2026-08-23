// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h" 
#include "AppAIControllerTowerBase.generated.h"

class UStateTreeAIComponent;
class UStateTree;


/**
 *
 */
UCLASS()
class PROJECT_PRINSI_API AAppAIControllerTowerBase : public AAIController
{
	GENERATED_BODY()

public:
	AAppAIControllerTowerBase();

protected:
	void OnPossess(APawn* InPawn)override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStateTreeAIComponent>StateTreeComp;		// 状态树组件

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Config")
	TObjectPtr<UStateTree>StateTree;					// 状态树
};
