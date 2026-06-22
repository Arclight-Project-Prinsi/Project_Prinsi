// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Prinsi/GameplayAbility/AppGA_AbilityAttackBase.h"
#include "AppGA_MeleeAttack_One.generated.h"

/**
 *
 */


UCLASS()
class PROJECT_PRINSI_API UAppGA_MeleeAttack_One : public UAppGA_AbilityAttackBase
{
	GENERATED_BODY()

public:
	UAppGA_MeleeAttack_One();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	)override;

	void OnAbilityStart()override;
	void OnAbilityFinished()override;

	void OnAttackMontageCompleted()override;	// 攻击AM完成时
	void OnAttackMontageInterrupted()override;	// 攻击AM异常中断时


	// ~~连击部分
protected:
	virtual void TryActivateNextCombo()override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility|AttackaAbility|Melee|Config")
	FGameplayTag NextComboAbilityTag;			// 下一段连击的Tag


	// ~~Cost部分
protected:
	// @note parameter
	virtual bool CheckCost(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		OUT FGameplayTagContainer* OptionalRelevantTags
	)const override;

	// @note parameter
	virtual void ApplyCost(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo
	)const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayAbility|AttackaAbility|Melee|Config|Cost")
	float CostMana = 5.0f;
};
