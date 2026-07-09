// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AppAnimNotify_ComboWindow.generated.h"

/**
 *
 */


UCLASS()
class PROJECT_PRINSI_API UAppAnimNotify_ComboWindow : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
