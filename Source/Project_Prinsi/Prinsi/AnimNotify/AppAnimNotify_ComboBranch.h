// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AppAnimNotify_ComboBranch.generated.h"

// @note
/**
 * @Notify 连击派生点
 *		- AM到此Notify时，玩家的连击可派生标识被激活。
 */


UCLASS()
class PROJECT_PRINSI_API UAppAnimNotify_ComboBranch : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
