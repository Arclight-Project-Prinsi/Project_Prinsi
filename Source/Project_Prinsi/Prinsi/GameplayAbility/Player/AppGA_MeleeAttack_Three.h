// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Prinsi/GameplayAbility/AppGA_AbilityBase.h"
#include "AppGA_MeleeAttack_Three.generated.h"

/**
 *
 */

class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;


UCLASS()
class PROJECT_PRINSI_API UAppGA_MeleeAttack_Three : public UAppGA_AbilityBase
{
	GENERATED_BODY()

public:
	UAppGA_MeleeAttack_Three();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	)override;

	void OnAbilityStart()override;

	void OnAbilityFinished()override;


	//ws----------------------------------
protected:
	UFUNCTION()
	void OnAttackMontageCompleted();

	UFUNCTION()
	void OnAttackMontageInterrupted();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayAbility|Melee|Config ")
	TObjectPtr<UAnimMontage> AttackMontage;			// @todo 攻撃AM配列(DAから読み取る？)

	// @note GA上にPlayMontageを1つのタスクとして認識される？
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait>CurrentPlayMontageTask;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayAbility|Melee|Config|Cost")
	float CostMana = 10.0f;

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

	//ws2----------------------------------------------------
//protected:
//	UPROPERTY(EditDefaultsOnly)
//	TSubclassOf<UGameplayAbility> NextComboAbility;		// 下一段连击的GA

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility|Melee|Config")
	FGameplayTag NextComboAbilityTag;		// 下一段连击的Tag
};
