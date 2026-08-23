// Fill out your copyright notice in the Description page of Project Settings.
#include "AppAIControllerTowerBase.h"
// Prinsi_Define
#include "Prinsi/Define/AppDefineDebug.h"		// Define_Debug工具
// Misc
#include "Components/StateTreeAIComponent.h"


AAppAIControllerTowerBase::AAppAIControllerTowerBase()
{
	StateTreeComp = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeComp"));

	// ~ 组件初始化 ~
	StateTreeComp->SetStartLogicAutomatically(false);
}

void AAppAIControllerTowerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const AAppTowerBase* Tower = Cast<AAppTowerBase>(InPawn);
	if (!Tower)
	{
		return;
	}

	// ~ ST组件启动 ~
	if (!StateTreeComp || !StateTree)
	{
		return;
	}
	StateTreeComp->SetStateTree(StateTree);
	StateTreeComp->StartLogic();

	//@ch
	APP_SCR_ERROR(TEXT("TowerAIC绑定成功。"));
}





