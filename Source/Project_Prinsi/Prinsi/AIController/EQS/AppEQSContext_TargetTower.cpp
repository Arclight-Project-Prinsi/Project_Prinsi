// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/AIController/EQS/AppEQSContext_TargetTower.h"
#include "EnvironmentQuery/EnvQueryTypes.h"				// Need_FEnvQueryInstance
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"		// @note 这个是?
#include "Prinsi/AIController/AppAIControllerCommon.h"
#include "Prinsi/Entity/Tower/AppTowerBase.h"			// Actor_Tower基类


void UAppEQSContext_TargetTower::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	// ~~获得Querier
	UObject* QueryOwenr = QueryInstance.Owner.Get();
	APawn* QuerierPawn = Cast<APawn>(QueryOwenr);
	if (!QuerierPawn)
	{
		return;
	}

	// ~从Querier处获得AIC
	AAppAIControllerCommon* AIController = Cast<AAppAIControllerCommon>(QuerierPawn->GetController());
	if (!AIController)
	{
		return;
	}

	// @todo 目前暂定以Tower为目标
	// ~~获得EQS的Target
	AAppTowerBase* TargetTower = AIController->TargetTower;
	if (!TargetTower)
	{
		return;
	}

	// @note 猜测 EQS的Context其实不关心Target是啥，它一律视为Actor，只是咱们让Context获得目标Acotor。
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetTower);

	// @todo 为何Editor的位置也显示是TargetTower?真就能映射上?
}
