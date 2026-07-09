// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Prinsi/GameplayAbility/AppGA_AbilityBase.h"
#include "AppGA_AbilityAttackBase.generated.h"

/**
 *
 */

class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;


UCLASS()
class PROJECT_PRINSI_API UAppGA_AbilityAttackBase : public UAppGA_AbilityBase
{
	GENERATED_BODY()

	// @todo
protected:
	UFUNCTION()
	virtual void OnAttackMontageCompleted() {};		// 攻击AM完成时

	UFUNCTION()
	virtual void OnAttackMontageInterrupted() {};	// 攻击AM异常中断时

	// @note
public:
	virtual void TryActivateNextCombo() {};

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayAbility|AttackaAbility|Config ")
	TObjectPtr<UAnimMontage> AttackMontage;			// @todo 通过DA来配置?

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait>CurrentPlayMontageTask;
};
