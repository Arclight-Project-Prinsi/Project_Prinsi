// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AppGA_AbilityBase.generated.h"

/**
 *
 */


UCLASS()
class PROJECT_PRINSI_API UAppGA_AbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	virtual void OnAbilityStart();		// Ability的Pre处理

	UFUNCTION()
	virtual void OnAbilityFinished();	// Ability的Post处理

	// @note 不使用Cue，直接在BP中调用视觉表现
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilityBase|Function")
	void BP_OnAbilityStart(AActor* Actor, const FVector& StartLocation);		// Ability的Pre处理（BP侧）

	UFUNCTION(BlueprintImplementableEvent, Category = "AbilityBase|Function")
	void BP_OnAbilityFinished(AActor* DashActor, const FVector& EndLocation);	// Ability的Post处理（BP侧）
};
